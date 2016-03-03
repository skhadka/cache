/*
	Ducksim: Cache: Replacement Policies
*/

#ifndef _REPLACEMENT_POLICIES_H
#define _REPLACEMENT_POLICIES_H	

#include "cache.h"
#include <time.h>
#include <stdlib.h>

typedef struct rp_stats_ {
	int max_replacement_ordering;
	double max_replacement_count;
} ReplacementStats;

typedef vector < vector < ReplacementStats > > ReplacementStatsBanks; //ReplacementStatsBanks[bank_id][set_id] => ReplacementStats


/*Helper function to implement replacement policy*/
int implement_replacement_policy (CacheLines replacement_lines, int policy_number);

/*Implement FIFO*/
int fifo(cache_lines replacement_lines, int number_banks);
#endif