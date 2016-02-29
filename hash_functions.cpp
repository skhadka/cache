/*
	Ducksim: Cache
*/

#include "hash_functions.h"

int hash_address(uint index, int virtual_bank_id, int number_cache_sets, bool set_associative) {
	int return_index = -1;
	if (set_associative) {
		return_index = index; //same index.. no hashing function needed
	}
	else {
		/*
			Implement Skewed Associative
			Different virtual banks will have different hash functions
		*/

	}
	return return_index;
}
