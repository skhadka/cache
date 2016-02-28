/*
	Ducksim: Cache Default values
*/

#include <map>
#include <string>	

const uint CACHE_SIZE = 65536; //64KB => tag=32-10-8=14
const int ASSOCIATIVITY = 1; //Direct Mapped=1, Fully Associative=0
const int BANKS = 1;
const int NUMBER_CACHE_LINES = 1024; //1024 lines => 10 bits
const int NUMBER_DATA_BLOCKS = 64; //Data Block = 64 bytes => 8 bits
const int REPLACEMENT_POLICY = 0; //random=0

const std::map < std::string, int > r_policies_map = {
	{"random", 0},
	{"fifo", 1}
};