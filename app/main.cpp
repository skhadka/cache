/*
Cache Simulator: Ducksim

Compile: g++ -std=c++11 *.cpp
Run: ./cache address_file #lines

*/

#include "../cache/include/cache.h"
#include <fstream>
#include <stdlib.h>

int main(int argc, char ** argv) {
	Cache default_cache = Cache();
	ifstream input_file(argv[1]);
	if (! input_file.is_open()) {
		cout << "\nError... Cannot open address file!!!" <<endl;
	}
	else {
		long lines = strtoul(argv[2], NULL, 0);
		default_cache.run(input_file, lines); //Run the 1st lines addresses in input_file on the cache, 0=complete run
		input_file.close();
		default_cache.dump_stats(cout);
		default_cache.dump_cache(cout);
	}
	return 0;
}