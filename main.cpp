/*
Cache Simulator: Ducksim

Compile: g++ -std=c++11 *.cpp
Run: ./cache address_file #lines

*/

#include "cache.h"
#include <fstream>
#include <stdlib.h>

vector <uint> make_address_vector(ifstream& stream, long lines) {
	vector <uint> addresses;
	char hex_address[11];
	if (lines <= 0) {
		while(stream.getline(hex_address, 11)) {
			uint address = (uint) strtol(hex_address, NULL, 0);
			addresses.push_back(address);
		}
	}
	else {
		for (int i=0; i<lines; ++i){
			stream.getline(hex_address, 11);
			uint address = (uint) strtol(hex_address, NULL, 0);
			addresses.push_back(address);
		}
	}
	return addresses;
}


int main(int argc, char ** argv) {
	Cache default_cache = Cache();
	ifstream input_file(argv[1]);
	if (! input_file.is_open()) {
		cout << "\nError... Cannot open address file!!!" <<endl;
	}
	else {
		long lines = strtoul(argv[2], NULL, 0);
		vector <uint> addresses = make_address_vector(input_file, lines);
		default_cache.run(addresses); //Run the 1st lines addresses in input_file on the cache, 0=complete run
		input_file.close();
		default_cache.dump_stats(cout);
		//default_cache.dump_cache(cout);
	}
	return 0;
}