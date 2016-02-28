/*
	Ducksim: Cache: Replacement Policies
*/

#include "replacement_policies.h"

int cache::get_replacement_line(int set_number) {
	
	int return_cache_line = set_number;

	if (this->associativity > 1) { /* Not DMC -- need to worry about replacement */ 
		int index = set_number * this->associativity;
		int max_index = index + this->associativity;

		/* Check for conflict i.e. check if all the set elements are valid */
		bool conflict = true; 
		while (index < max_index) {
			if (! this->cache_lines[index].valid) {
				conflict = false;
				return_cache_line = index;
				index = max_index; //found invalid line => STOP & use that for replacement 
			}
			index += 1;
		} 
		/*If conflict then need to implement replacement policy*/
		if (conflict) {
			return_cache_line = implement_replacement_policy(index, max_index, this->replacement_policy);
			this->stats.replacements += 1; //something was replaced
		}
	}
	//cout << "replacement: "<<return_cache_line<<endl;
	return return_cache_line;
}


int cache::implement_replacement_policy(int index, int max_index, int policy_number) {
	srand(time(0));
	int return_cache_line = -1;
	switch (policy_number) {
		case 0:
			/*Random Replacement Policy*/
			return_cache_line = rand() % this->associativity;
			break;
		default:
			cout << "Invalid Replacement Policy!" <<endl;
			exit(1);
	}
	return return_cache_line;
}