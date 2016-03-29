/*
	Ducksim: Cache: Implement Cache Replacement Policies
*/

#ifndef _REPLACEMENT_POLICIES_H
#define _REPLACEMENT_POLICIES_H	

#include <time.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "types.h"


/*Vector to keep track of which set index is being considered for replacement in which bank*/
typedef vector <int> ReplacementLines; 

/* Keep track of Stats needed per cache line for Replacement Policy Implementation*/
struct ReplacementStats {
	int placement_order;	//FIFO: Relative 1st placemement ordering, 1=newly replaced
	int access_order;		//LRU: Relative freshest access ordering, 1=newly accessed
	double count;	//Policies that need count.. Most/Least Frequently used.. expand later
};

/*Large Bank: Mirroring cache bank to keep track of stats for implementing Replacement Policies*/
typedef vector < vector < ReplacementStats > > ReplacementStatsBank; //ReplacementStatsBank[bank_id][set_id] => ReplacementStats

/*Do all Cache Replacement Stuff*/
class CacheReplacementStats {
	public:
		/*Initialize the replacement bank*/
		CacheReplacementStats(int number_banks, int number_cache_sets);
		~CacheReplacementStats();
		
		/*ReInit ReplacementBank aka set all stats to 0*/
		void reinit();

		/*Helper function to implement given replacement policy*/
		int implement_replacement_policy (ReplacementLines replacement_lines, ReplacementPolicy policy_number);

		/*Update the Replacement Bank in each access*/
		void update(ReplacementLines replacement_lines, int number_banks, int bank_id, int set_id, bool hit);

		/*Dump the replacement_bank*/
		void dump_replacement_table(ostream& stream);

	private:
		int total_banks;
		int total_sets;
		/*Table of all stats needed for implementing different Replacement Policies*/
		ReplacementStatsBank replacement_bank;


		/* Policies Implementation : Retun the VirtualBankID for replacement*/
		int policy_random(int number_banks);
		int policy_fifo(ReplacementLines replacement_lines, int number_banks);
		int policy_lru(ReplacementLines replacement_lines, int number_banks);
};


#endif