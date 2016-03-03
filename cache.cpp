/*
	Ducksim: Cache
*/

#include "cache.h"
#include "hash_functions.h"


cache::cache(uint in_size, int in_associativity, int in_banks, int in_number_cache_lines, int in_number_data_blocks, int in_replacement_policy) 
: size(in_size), associativity(in_associativity), banks(in_banks), 
number_cache_lines(in_number_cache_lines), number_data_blocks(in_number_data_blocks),
replacement_policy(in_replacement_policy)
{
	if (in_associativity==0) this->associativity = this->number_cache_lines; //Fully Associative
	this->number_virtual_banks = this->associativity;
	this->number_cache_sets = this->number_cache_lines / this->associativity; 

	for (int vbanks=0; vbanks<this->number_virtual_banks; ++vbanks) {
		cache_lines bank;
		for (int i=0; i<this->number_cache_sets; ++i) {
			/*Initialize Cache Line*/
			cache_line line = {
				0, 			//tag
				false, 	//valid
				false, 	//dirty
				in_number_data_blocks, //number_data_blocks
				
				0,		//replacement_ordering
				0,		//access_count
			};
			bank.push_back(line);
		}
		this->virtual_banks.push_back(bank); //push each bank in the virtual bank vector
	}

	/*Initialize Cache Stats*/

	this->stats = (cache_statistics){
		0,	//hits
		0,	//misses
		0, 	//access 
		0,	//replacements
		0,	//write_backs
		0,	//bandwidth
	};

	/*Initilize upper and lower levels*/
	this->upper_level = NULL;
	this->lower_level = NULL;
}

cache::~cache() {
	//nothing malloced yet!
}

/* Set all valid & dirty bits to 0 for cache reinitialization */
void cache::reinit_cache(){
	for (int i=0; i<this->number_virtual_banks; ++i) {
		for (int j=0; j<this->number_cache_sets; ++j) {
			this->virtual_banks[i][j].valid = false;
			this->virtual_banks[i][j].dirty = false;
			this->virtual_banks[i][j].replacement_ordering = 0;
		}
	}
}

/* Check if an address is in the cache and update hit/miss counts accordingly */
bool cache::access(uint address, bool write) {

	int offset = address % this->number_data_blocks; //will need this when actually reading/writing data
	address = address / this->number_data_blocks; //get rid of block offset
	uint access_set_tag = address / this->number_cache_sets; //get rid of index for the tag
	uint access_index = address % this->number_cache_sets; //get the actual index of the address
	/*Start access stats*/
	this->stats.access += 1;
	bool hit = false;

	/*Check is the tag is any one of lines in the set*/
	//int access_index = access_set_index * this->associativity;
	//int max_access_index = access_index + this->associativity;

	cache_lines replacement_lines; //keep track of lines to be replaced so that replacement policy can select which one of them to replace later on
	vector <int> replacement_indexes; 

	/*Final values*/
	int cache_bank_id = -1;
	int cache_index = -1;

	for (int vbank=0; vbank<this->number_virtual_banks; ++vbank) {
		int access_set_index = hash_address(access_index, vbank, this->number_cache_sets, true); //get the set index where the index is mapped to in each bank : Rightnow just set associative
		cout <<"VBank\tTag\tSIndex\tOffset"<<endl;
		cout <<vbank<<"\t"<<access_set_tag <<"\t" <<access_set_index <<"\t" <<offset <<endl;
		if (this->virtual_banks[vbank][access_set_index].valid && (this->virtual_banks[vbank][access_set_index].tag == access_set_tag)) {//hit condition
				hit = true;
				cache_bank_id = vbank;
				cache_index = access_set_index;
				this->virtual_banks[vbank][access_set_index].dirty = write; //it's been written to now.. so set the dirty bit -- need to implement the write policy here.. later
				break;
		}
		replacement_lines.push_back(this->virtual_banks[vbank][access_set_index]); 
		replacement_indexes.push_back(access_set_index);
	}
	
	/* Implement Read policies on miss? Read through? */

	/*Update cache stats*/
	if (hit) this->stats.hits += 1;
	else {
		/* Address not in Cache => Implement Replacement Policy => Find out which cache line to replace */
		int replacement_bank_id = get_replacement_line(replacement_lines); //replacement policy selects which virtual bank to replace cache line from
		int replacement_set_index = replacement_indexes[replacement_bank_id];
		 
		
		this->virtual_banks[replacement_bank_id][replacement_set_index].tag = access_set_tag;
		this->virtual_banks[replacement_bank_id][replacement_set_index].valid = true;
		this->stats.misses += 1;

		cache_bank_id = replacement_bank_id;
		cache_index = replacement_set_index;
	}

	/*Update Ordering and Count for Replacement Policies*/
	this->virtual_banks[cache_bank_id][cache_index].replacement_ordering = 1; //most recently used => least order = 1 
	this->virtual_banks[cache_bank_id][cache_index].access_count += 1;

	return hit;
}

/*Dump all cache stats*/
void cache::dump_stats(ostream& stream) {
	stream << "\n------------------- Cache Stats -------------------"<<endl;
	stream << "Hit Rate: "<<(double)this->stats.hits/this->stats.access<<endl;
	stream << "Miss Rate: "<<(double)this->stats.misses/this->stats.access<<endl;
	stream << "Replacement Rate: "<<(double)this->stats.replacements/this->stats.access<<endl;
	stream << "Write Back Rate: "<<(double)this->stats.write_backs/this->stats.access<<endl;
	stream << endl;
	stream << "Hits: "<<this->stats.hits<<endl;
	stream << "Misses: "<<this->stats.misses<<endl;
	stream << "Replacements: "<<this->stats.replacements<<endl;
	stream << "Total Accesses: "<<this->stats.access;	
	stream << "\n---------------------------------------------------"<<endl<<endl;
}

/*Dump all cache settings*/
void cache::dump_settings(ostream& stream) {

}

/*Dump Current Cache State*/
void cache::dump_cache(ostream& stream) { /*Will need more changes with skewed associative as we need to rehash to get back original index from the hashed index*/
	stream << "\n------------------- Cache State -------------------"<<endl;
	stream <<"Line\tBank\tSetIndex\tTag\tValid\tData"<<endl;
	for (int i=0; i<this->number_cache_lines; ++i) {
		int bank_id = i % this->number_virtual_banks; 
		int set_index = i / this->number_virtual_banks; /*Need to rehash this to get actual index back for skewed associative!!!*/
		uint set_tag = this->virtual_banks[bank_id][set_index].tag;
		uint block_start_address = (set_tag * this->number_cache_sets * this->number_data_blocks) //tag
			+ (set_index*this->number_data_blocks) //index
			+ 0; //offset
		uint block_end_address = block_start_address //starting address
			+ this->number_data_blocks; //offset

		/*Output Cache Line*/	
		stream<<i<<"\t"<<bank_id<<"\t"<<set_index<<"\t\t"<<set_tag<<"\t"<<this->virtual_banks[bank_id][set_index].valid;
		stream<<"\t"<<block_start_address<<" :: "<<block_end_address<<endl;
	}
	stream << "--------------------------------------------------"<<endl;	
}

bool cache::run(istream& stream, int lines) {
	char hex_address[11];
	if (lines <= 0) {
		while(stream.getline(hex_address, 11)) {
			uint address = (uint) strtol(hex_address, NULL, 0);
			bool hit = access(address, false);
		}
	}
	else {
		for (int i=0; i<lines; ++i){
			stream.getline(hex_address, 11);
			uint address = (uint) strtol(hex_address, NULL, 0);
			cout<<"\nAddress: "<<hex_address<<", "<< address<< endl;
			bool hit = access(address, false);
			if (hit) cout << "<< HIT!! >>"<<endl;
			else cout << "<< MISS!! >>"<<endl;
		}
	}
	return true;
}
