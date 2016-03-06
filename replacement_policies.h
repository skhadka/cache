/*
	Ducksim: Cache: Replacement Policies
*/

#ifndef _REPLACEMENT_POLICIES_H
#define _REPLACEMENT_POLICIES_H	

#include <time.h>
#include <stdlib.h>

#include "cache.h"

/* Keep track of Stats needed per cache line for Replacement Policy Implementation*/
typedef struct ReplacementStats {
	int placement_order;	//FIFO: Relative 1st placemement ordering, 1=newly replaced; LRU: Relative freshest access ordering, 1=newly accessed
	double placement_count;	//Policies that need count.. Most/Least Frequently used.. expand later
};

typedef vector < vector < ReplacementStats > > ReplacementStatsBanks; //ReplacementStatsBanks[bank_id][set_id] => ReplacementStats

class CacheReplacementStats {
	public:
		/*Helper function to implement given replacement policy*/
		int implement_replacement_policy (CacheLines replacement_lines, ReplacementPolicy policy_number);

	private:
		/*Table of all stats needed for implementing different Replacement Policies*/
		ReplacementStatsBanks replacement_bank;

		/*Implementations: Retun the VirtualBankID for replacement*/
		inline int random(int number_banks);
		int fifo(cache_lines replacement_lines, int number_banks);


};





#endif