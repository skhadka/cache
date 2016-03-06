/*
	Ducksim: Cache: Implement Cache Replacement Policies
*/

#include "replacement_policies.h"

CacheReplacementStats::CacheReplacementStats(int number_banks, int number_cache_sets){
	
	/*Init helper stats*/
	this->total_banks = number_banks;
	this->total_sets = number_cache_sets;

	if (number_banks <= 1) {
		//No need to keep a replacement table
		return;
	}

	vector < ReplacementStats > stats_list;
	for (int set=0; set<number_cache_sets; set++) { 
		ReplacementStats element = {
			0,	//placement_order
			0,	//access_order
			0,	//count
		};
		stats_list.push_back(element);
	}
	/*Initialize Replacement Bank*/
	for (int bank=0; bank<number_banks; ++bank) {	
		this->replacement_bank.push_back(stats_list);
	}
	/*Initialize Current Stats List*/
	//this->current_stats.push_back(stats_list);
}

CacheReplacementStats::~CacheReplacementStats() {
	/* Nothing to kill yet!! */
}

void CacheReplacementStats::reinit() {
	for (int i=0; i<this->total_banks; ++i) {
		for (int j=0; j<this->total_sets; ++j) {
			this->replacement_bank[i][j].placement_order = 0;
			this->replacement_bank[i][j].access_order = 0;
			this->replacement_bank[i][j].count = 0;
		}
	}
}

int CacheReplacementStats::implement_replacement_policy (ReplacementLines replacement_lines, ReplacementPolicy policy_number) {
	srand(time(0));
	int return_bank_id = -1;
	int number_banks = replacement_lines.size();
	switch (policy_number) {
		case 0:
			/*Random Replacement Policy*/
			return_bank_id = policy_random(number_banks);
			break;
		case 1:
			/*FIFO Replacement Policy*/
			return_bank_id = policy_fifo(replacement_lines, number_banks);
			break;
		default:
			cout << "Invalid Replacement Policy!" <<endl;
			exit(1);
	}
	return return_bank_id;	
}

void CacheReplacementStats::update(ReplacementLines replacement_lines, int number_banks, int bank_id, int set_id, bool hit) {
	if (number_banks <= 1) return; //No need to implement any replacement policies here
	cout << "In RP update: Bank, Set=> \t"<<bank_id<<" , "<<set_id<<endl;
	int current_placement_order = this->replacement_bank[bank_id][set_id].placement_order;
	int current_access_order = this->replacement_bank[bank_id][set_id].access_order;
	double current_count = this->replacement_bank[bank_id][set_id].count; //keep track of max_count for normalization?

	/*Access Ordering is changed per access => Increase all values and set the input ones to 1*/
	cout << "before loop" <<endl;
	for (int i=0; i<number_banks ; ++i) {
		int set = replacement_lines[i];

		cout << "RP bank, set: "<<i <<","<<set<<endl;

		if( (replacement_bank[i][set].access_order > 0) && (replacement_bank[i][set].access_order < current_access_order) ) {
			cout << "access"<<endl;
			this->replacement_bank[i][set].access_order += 1;
		}
		
		if (!hit && (replacement_bank[i][set].placement_order > 0) && (replacement_bank[i][set].placement_order < current_placement_order) ) {
			/* New placement on the cache => update the placement ordering of all the elements that were placed before it*/
			cout<<"placement"<<endl;
			this->replacement_bank[i][set].placement_order += 1; 
		}
		//}
	}

	/*Update values for the accessed lines*/
	cout<<"out of loop"<<endl;
	if (!hit) this->replacement_bank[bank_id][set_id].placement_order = 1;
	this->replacement_bank[bank_id][set_id].access_order = 1;
	//this->replacement_bank[bank_id][set_id].count = 1; -- Do something eventually	

}

/************* Replacement Policies ********************/

/*Random*/
int CacheReplacementStats::policy_random(int number_banks) { 
	int return_bank_id = rand() % number_banks; 
	cout<< "Replacement Bank: "<<return_bank_id<<endl;
	return return_bank_id;
}

/*FIFO*/
int CacheReplacementStats::policy_fifo(ReplacementLines replacement_lines, int number_banks) {
	int return_bank_id = -1;
	for (int i=0; i<number_banks; ++i) {
		int set = replacement_lines[i];
		if (this->replacement_bank[i][set].placement_order == number_banks ) { /*Oldest Cacheline Found!! Replace it!!*/
			return_bank_id = i;
			break;
		}
	}
	cout<< "Replacement Bank: "<<return_bank_id<<endl;
	return return_bank_id;
}

/*LRU*/
int CacheReplacementStats::policy_lru(ReplacementLines replacement_lines, int number_banks) {
	int return_bank_id = -1;
	for (int i=0; i<number_banks; ++i) {
		int set = replacement_lines[i];
		if (this->replacement_bank[i][set].access_order == number_banks ) { /*LRU Cacheline Found!! Replace it!!*/
			return_bank_id = i;
			break;
		}
	}
	cout<< "Replacement Bank: "<<return_bank_id<<endl;
	return return_bank_id;
}