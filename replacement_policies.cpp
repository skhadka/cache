/*
	Ducksim: Cache: Replacement Policies
*/

#include "replacement_policies.h"

int cache::get_replacement_line( cache_lines replacement_lines ) {
	
	int return_bank_id = 0;

	if (this->associativity != 1) { /* Not DMC -- need to worry about replacement policies*/ 
		
		/* Check for conflict i.e. check if all the replacement lines are valid */
		bool conflict = true; 
		int vbank = 0;
		while (vbank < this->number_virtual_banks) {
			if (! replacement_lines[vbank].valid) {
				conflict = false;
				return_bank_id = vbank;
				vbank = this->number_virtual_banks; //found invalid line => STOP & use that for replacement 
			}
			vbank += 1;
		} 
		/*If conflict then need to implement replacement policy*/
		if (conflict) {
			return_bank_id = implement_replacement_policy(replacement_lines, this->replacement_policy);
			this->stats.replacements += 1; //something was replaced
		}
		else { /*No Conflit => empty */

		}
	}
	else { //Just 1 virtual bank for DMC
		return_bank_id = 0;
	}

	return return_bank_id;
}

/*
int cache::implement_replacement_policy(int index, int max_index, int policy_number) {
	srand(time(0));
	int return_cache_line = -1;
	switch (policy_number) {
		case 0:
			//Random Replacement Policy
			return_cache_line = rand() % this->associativity;
			break;
		default:
			cout << "Invalid Replacement Policy!" <<endl;
			exit(1);
	}
	return return_cache_line;
}*/

int implement_replacement_policy (cache_lines replacement_lines, int policy_number) {
	srand(time(0));
	int return_bank_id = -1;
	int number_banks = replacement_lines.size();
	switch (policy_number) {
		case 0:
			/*Random Replacement Policy*/
			return_bank_id = rand() % number_banks;
			break;
		case 1:
			/*FIFO Replacement Policy*/
			return_bank_id = fifo(replacement_lines, number_banks);
			break;
		default:
			cout << "Invalid Replacement Policy!" <<endl;
			exit(1);
	}
	return return_bank_id;	
}

int fifo(cache_lines replacement_lines, int number_banks) {
	int return_bank_id = -1;
	for (int i=0; i<number_banks; ++i) {
		cout << "Replacement Ordering For Bank "<<i<<": "<<replacement_lines[i].replacement_ordering<<endl;
		if (replacement_lines[i].replacement_ordering >= number_banks) return_bank_id = i;
		replacement_lines[i].replacement_ordering += 1; 
	}
	cout<< "Replacement Bank: "<<return_bank_id<<endl;
	return return_bank_id;
}