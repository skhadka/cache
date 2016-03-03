/*
	Ducksim: Cache: Replacement Policies
*/

#ifndef _REPLACEMENT_POLICIES_H
#define _REPLACEMENT_POLICIES_H	

#include "cache.h"
#include <time.h>
#include <stdlib.h>

/*Helper function to implement replacement policy*/
int implement_replacement_policy (CacheLines replacement_lines, int policy_number);

#endif