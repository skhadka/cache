/*
	Ducksim: Cache
*/

#include <stdlib.h>
#include "cache.h"
#include "hash_functions.h"


Cache::Cache(uint in_size, int in_associativity, int in_banks, int in_number_cache_lines,
             int in_number_data_blocks, ReplacementPolicy in_replacement_policy)
    : size(in_size)
    , associativity(in_associativity)
    , banks(in_banks)
    , number_cache_lines(in_number_cache_lines)
    , number_data_blocks(in_number_data_blocks)
    , replacement_policy(in_replacement_policy)
{
	if (in_associativity==0) this->associativity = this->number_cache_lines; //Fully Associative
	this->number_virtual_banks = this->associativity;
	this->number_cache_sets = this->number_cache_lines / this->associativity; 

	for (int vbanks=0; vbanks<this->number_virtual_banks; ++vbanks) {
		//vector <int> replacement_ordering_list;
		CacheLines bank;
		for (int i=0; i<this->number_cache_sets; ++i) {
			/*Initialize Cache Line*/
			CacheLine line = {
				0, 			//tag
				false, 	//valid
				false, 	//dirty
				in_number_data_blocks, //number_data_blocks
				
				//0,		//replacement_ordering
				//0,		//access_count
			};
			bank.push_back(line);
			//replacement_ordering_list.push_back(0);
		}
		this->virtual_banks.push_back(bank); //push each bank in the virtual bank vector
		//this->max_replacement_ordering.push_back(replacement_ordering_list);
	}

	/*Initialize Cache Stats*/
	this->stats = (CacheStatistics){
		0,	//hits
		0,	//misses
		0, 	//access 
		0,	//replacements
		0,	//write_backs
		//0,	//bandwidth
	};

	/*Initilize Replacement Stats*/
	this->replacement_stats = new CacheReplacementStats(this->number_virtual_banks, this->number_cache_sets);

	/*Initilize upper and lower levels*/
	this->upper_level = NULL;
	this->lower_level = NULL;
}

Cache::~Cache() {
	delete this->replacement_stats;
	//this->replacement_stats = NULL;
}

/* Set all valid & dirty bits to 0 for cache reinitialization */
void Cache::reinit_cache(){

	/*Reinitialize the Cache*/
	for (int i=0; i<this->number_virtual_banks; ++i) {
		for (int j=0; j<this->number_cache_sets; ++j) {
			this->virtual_banks[i][j].valid = false;
			this->virtual_banks[i][j].dirty = false;
		}
	}

	/*Reinitialize the Replacement Stats*/
}

/* Check if an address is in the cache and update hit/miss counts accordingly */
bool Cache::access(uint address, bool write) {

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

	CacheLines replacement_lines; //keep track of lines to be replaced so that replacement policy can select which one of them to replace later on
	ReplacementLines replacement_indexes;

	/*Final values*/
	int cache_bank_id = -1;
	int cache_set_id = -1;

	for (int vbank=0; vbank<this->number_virtual_banks; ++vbank) {
		int access_set_index = hash_address(access_index, vbank, this->number_cache_sets, true); //get the set index where the index is mapped to in each bank : Rightnow just set associative
		cout <<"VBank\tTag\tSIndex\tOffset"<<endl;
		cout <<vbank<<"\t"<<access_set_tag <<"\t" <<access_set_index <<"\t" <<offset <<endl;
		if (this->virtual_banks[vbank][access_set_index].valid && (this->virtual_banks[vbank][access_set_index].tag == access_set_tag)) {//hit condition
				hit = true;
				cache_bank_id = vbank;
				cache_set_id = access_set_index;
				this->virtual_banks[vbank][access_set_index].dirty = write; //it's been written to now.. so set the dirty bit -- need to implement the write policy here.. later
				//break; //can't break anymore -- need to build full replacement_indexes to be used for updating the replacement table
		}
		replacement_lines.push_back(this->virtual_banks[vbank][access_set_index]); 
		replacement_indexes.push_back(access_set_index);
	}
	
	/* Implement Read policies on miss? Read through? */

	/*Update cache stats*/
	if (hit) this->stats.hits += 1;
	else {
		/* Address not in Cache => Implement Replacement Policy => Find out which cache line to replace */
		int replacement_bank_id = get_replacement_line(replacement_lines, replacement_indexes); //replacement policy selects which virtual bank to replace cache line from
		int replacement_set_index = replacement_indexes[replacement_bank_id];
		 
		
		this->virtual_banks[replacement_bank_id][replacement_set_index].tag = access_set_tag;
		this->virtual_banks[replacement_bank_id][replacement_set_index].valid = true;
		this->stats.misses += 1;

		cache_bank_id = replacement_bank_id;
		cache_set_id = replacement_set_index;
	}

	/*Update Replacement Stats*/
	(this->replacement_stats)->update(replacement_indexes, this->number_virtual_banks, cache_bank_id, cache_set_id, hit);

	return hit;
}

/*Dump all cache stats*/
void Cache::dump_stats(ostream& stream) {
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
void Cache::dump_settings(ostream& stream) {

}

/*Dump Current Cache State*/
void Cache::dump_cache(ostream& stream) { /*Will need more changes with skewed associative as we need to rehash to get back original index from the hashed index*/
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

bool Cache::run(istream& stream, int lines) {
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

int Cache::get_replacement_line(CacheLines cache_replacement_lines, ReplacementLines replacement_lines ) {
	
	int return_bank_id = -1;

	if (this->associativity != 1) { /* Not DMC -- need to worry about replacement policies*/ 
		
		/* Check for conflict i.e. check if all the replacement lines are valid */
		bool conflict = true; 
		int vbank = 0;
		while (vbank < this->number_virtual_banks) {
			if (! cache_replacement_lines[vbank].valid) {
				conflict = false;
				return_bank_id = vbank;
				vbank = this->number_virtual_banks; //found invalid line => STOP & use that for replacement 
			}
			vbank += 1;
		} 
		/*If conflict then need to implement replacement policy*/
		if (conflict) {
			return_bank_id = (this->replacement_stats)->implement_replacement_policy(replacement_lines, this->replacement_policy);
			this->stats.replacements += 1; //something was replaced
		}
		else { 
			/* No Conflit => already found a replacement => nothing else to do.. */
		}
	}
	else { //Just 1 virtual bank for DMC
		return_bank_id = 0;
	}

	return return_bank_id;
}
