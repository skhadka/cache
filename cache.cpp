/*
	Ducksim: Cache
*/

#include "cache.h"

cache::cache(uint in_size, int in_associativity, int in_banks, int in_number_cache_lines, int in_number_data_blocks, int in_replacement_policy) 
: size(in_size), associativity(in_associativity), banks(in_banks), 
number_cache_lines(in_number_cache_lines), number_data_blocks(in_number_data_blocks),
replacement_policy(in_replacement_policy)
{
	for (int i=0; i<this->number_cache_lines; ++i) {
		
		if (in_associativity==0) this->associativity = this->number_cache_lines; //Fully Associative
		this->number_cache_sets = this->number_cache_lines / this->associativity; 
		/*Initialize Cache Line*/
		cache_line line = {
			0, 			//tag
			false, 	//valid
			false, 	//dirty
			in_number_data_blocks, //number_data_blocks
		};
		this->cache_lines.push_back(line);

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
}

cache::~cache() {
	//nothing malloced yet!
}

/* Set all valid & dirty bits to 0 for cache reinitialization */
void cache::reinit_cache(){
	for (int i=0; i<this->number_cache_lines; ++i) {
		this->cache_lines[i].valid = false;
		this->cache_lines[i].dirty = false;
	}
}

/* Check if an address is in the cache and update hit/miss counts accordingly */
bool cache::access(uint address, bool write) {
	int offset = address % this->number_data_blocks; //will need this when actually reading/writing data
	address = address / this->number_data_blocks; //get rid of block offset
	int access_set_index = address % this->number_cache_sets; //will have to replace this with hash function eventually
	uint access_set_tag = address / this->number_cache_sets; //get rid of index for the tag
	cout <<"Tag\tSIndex\tOffset"<<endl;
	cout <<access_set_tag <<"\t" <<access_set_index <<"\t" <<offset <<endl;

	/*Start access stats*/
	this->stats.access += 1;
	bool hit = false;

	/*Check is the tag is any one of lines in the set*/
	int access_index = access_set_index * this->associativity;
	int max_access_index = access_index + this->associativity;
	while (access_index < max_access_index) {
		if (this->cache_lines[access_index].valid && (this->cache_lines[access_index].tag == access_set_tag)) {//hit condition
				hit = true;
				access_index =  max_access_index;//element found in cache => stop
				this->cache_lines[access_index].dirty = write; //it's been written to now.. so set the dirty bit -- need to implement the write policy here.. later
		}
		access_index += 1;
	}
	/* Implement Read policies on miss? Read through? */

	/*Update cache stats*/
	if (hit) this->stats.hits += 1;
	else {
		/* Address not in Cache => Implement Replacement Policy => Find out which cache line to replace */
		int replacement_line = get_replacement_line(access_set_index); //get replacement line in the set
		this->cache_lines[replacement_line].tag = access_set_tag;
		this->cache_lines[replacement_line].valid = true;
		this->stats.misses += 1;
	}
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
void cache::dump_cache(ostream& stream) {
	stream << "\n------------------- Cache State -------------------"<<endl;
	stream <<"Line\tSet\tSetIndex\tTag\tValid\tData"<<endl;
	for (int i=0; i<this->number_cache_lines; ++i) {
		int set_number = i / this->associativity; 
		int set_index = i % this->associativity;
		uint block_start_address = (this->cache_lines[i].tag * this->number_cache_sets * this->number_data_blocks) //tag
			+ (i*this->number_data_blocks) //index
			+ 0; //offset
		uint block_end_address = block_start_address //starting address
			+ this->number_data_blocks; //offset

		/*Output Cache Line*/	
		stream<<i<<"\t"<<set_number<<"\t"<<set_index<<"\t\t"<<this->cache_lines[i].tag<<"\t"<<this->cache_lines[i].valid;
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
