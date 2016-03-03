/*
	Ducksim: Cache: Replacement Policies
*/

#ifndef _REPLACEMENT_POLICIES_H
#define _REPLACEMENT_POLICIES_H	

#include "cache.h"
#include <time.h>
#include <stdlib.h>



/*Helper function to implement replacement policy*/
int implement_replacement_policy (cache_lines replacement_lines, int policy_number);

/*Implement FIFO*/
int fifo(cache_lines replacement_lines, int number_banks);
#endif