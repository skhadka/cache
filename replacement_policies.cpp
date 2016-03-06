/*
	Ducksim: Cache: Replacement Policies
*/

#include "replacement_policies.h"


int implement_replacement_policy (CacheLines replacement_lines, ReplacementPolicy policy_number) {
	srand(time(0));
	int return_bank_id = -1;
	int number_banks = replacement_lines.size();
	switch (policy_number) {
		case 0:
			/*Random Replacement Policy*/
			return_bank_id = random(number_banks);
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

inline int random(int number_banks) { 
	return rand() % number_banks; 
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