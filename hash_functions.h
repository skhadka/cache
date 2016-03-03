/*
	Ducksim: Cache
*/

#ifndef _HASH_FUNCTIONS_H
#define _HASH_FUNCTIONS_H

#include <iostream>

#include "types.h"

/*Return which cache line to access for each virtual bank -- just a mod function for set associative*/	
int hash_address(uint index, int virtual_bank_id, int number_cache_sets, bool set_associative);

#endif
