/*
	Ducksim: Cache
*/

#ifndef _CACHE_H
#define _CACHE_H

#include <iostream>
#include <math.h>
#include <vector>	
#include <iomanip>	
#include "defaults.h"


using namespace std;

typedef struct cache_line_ {
	uint tag;
	bool valid;
	bool dirty;
	int number_data_blocks; //in B
} cache_line;

typedef struct cache_statistics_ {
	uint hits;
	uint misses;
	uint access; //total memory references 
	uint replacements; //total number of cache line replacements
	uint write_backs; //total number of write backs
	double bandwidth;
} cache_statistics;


class cache {
	
	public: 
		/*Default Constructor*/
		cache(uint in_size=CACHE_SIZE, int in_associativity=ASSOCIATIVITY, int in_banks=BANKS, 
			int in_number_cache_lines=NUMBER_CACHE_LINES, int in_number_data_blocks=NUMBER_DATA_BLOCKS, 
			int in_replacement_policy=REPLACEMENT_POLICY);
		
		/*Default Destructor*/
		~cache();

		/*Reinitialize cache usually after a program is complete*/
		void reinit_cache();

		/*Cache Access with memory address and read/write condition => implement replacement + placement policy and do the bookeeping
		Return true on hit*/
		bool access(uint address, bool write);

		/*Dump cache statistics on the output stream*/
		void dump_stats(ostream& stream);

		/*Dump cache settings on the output stream*/
		void dump_settings(ostream& stream);

		/*Dump current cache state on the Output Stream*/
		void dump_cache(ostream& stream);

		/*Run cache based on given input stream for #lines, -1 means complete run*/
		bool run(istream& stream, int lines=-1); //run cache from stream.. can do cin as well

		/*Return the cache line to be replacement based on the replacement policy*/
		int get_replacement_line( int set_number );

		/*In case a line in the set needs to be replaced */
		int implement_replacement_policy(int index, int max_index, int policy_number); 
	
	private:
		uint size; //in B
		int associativity; //should be to the power of 2
		int banks; //just use 1 for now
		int number_cache_lines; //total number
		int number_cache_sets;
		int number_data_blocks; //per cache line
		vector <cache_line> cache_lines;
		cache_statistics stats; 
		cache *lower_level;	
		cache *upper_level; 
		int replacement_policy;
		/*need hash functions for associativity later*/ 
};

#endif
