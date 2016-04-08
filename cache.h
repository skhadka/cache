/*
	Ducksim: Cache
*/

#ifndef _CACHE_H
#define _CACHE_H

#include <iostream>
#include <math.h>
#include <vector>	
#include <iomanip>
#include <fstream>	

#include "defaults.h"
#include "types.h"
#include "replacement_policies.h"	

//using namespace std;

/* Cacheline */
struct CacheLine {
	uint tag;
	bool valid;
	bool dirty;
	int number_data_blocks; //in B

	/*Some pointer to actual data block ==> PENDING....*/
	
};

/* CacheStats per cache */
struct CacheStatistics {
	uint hits;
	uint misses;
	uint access; //total memory references 
	uint replacements; //total number of cache line replacements
	uint write_backs; //total number of write backs
};

/*
 * CacheLines is a collection of CacheLine objects, held in a vector.
 */
typedef vector<CacheLine> CacheLines;


/*
 * Describes a cache
 */
class Cache {
	
	public: 
		/*Default Constructor*/
		Cache(uint in_size=CACHE_SIZE, int in_associativity=ASSOCIATIVITY, int in_banks=BANKS,
			int in_number_cache_lines=NUMBER_CACHE_LINES, int in_number_data_blocks=NUMBER_DATA_BLOCKS, 
			ReplacementPolicy in_replacement_policy=REPLACEMENT_POLICY);
		
		/*Default Destructor*/
		~Cache();

		/*Reinitialize cache usually after a program is complete*/
		void reinit_cache();

		/*Cache Access with memory address and read/write condition => implement replacement + placement policy and do the bookeeping
		Return true on hit*/
		bool access(uint address, bool write);

		/*Dump cache statistics on the Output Stream*/
		void dump_stats(ostream& stream);

		/*Dump cache settings on the Output Stream*/
		void dump_settings(ostream& stream);

		/*Dump current cache state on the Output Stream*/
		void dump_cache(ostream& stream);

		/*Run cache based on given input stream for #lines, -1 means complete run*/
		bool run(vector <uint> addresses); //run cache from stream.. can do cin as well

		/*Get back the virtual bank id to be replaced which is the index of the replacement_line*/
		int get_replacement_line( CacheLines cache_replacement_lines, ReplacementLines replacement_lines);

		/*Implement Replacement Policy*/
	

	private:

		/*Configuration*/
		uint size; //in B
		int associativity; //should be to the power of 2
		int number_virtual_banks; //keep track of virtual banks for implementing associativity 
		int banks; //just use 1 for now
		int number_cache_lines; //total number
		int number_cache_sets;  //total number / associativity or virtual banks
		int number_data_blocks; //per cache line
		vector<CacheLines> virtual_banks; //bank = collection of cache lines that belong to different sets
		
		/*Stats*/
		CacheStatistics stats;

		/*Replacement Policy + Stat for implementing Replacement Policy*/
		ReplacementPolicy replacement_policy;
		CacheReplacementStats *replacement_stats; 
		
		/* Memory Hiearchy */
		Cache *lower_level;
    	Cache *upper_level;

		/*need hash functions for associativity later: per bank*/ 
};

#endif
