/*
	Ducksim: Cache Default values
*/

#include <map>
#include <string>
#include "types.h"



const uint CACHE_SIZE = 65536; //64KB => tag=32-10-8=14
const int ASSOCIATIVITY = 2; //Direct Mapped=1, Fully Associative=0
//const int VIRTUAL_BANKS = ASSOCIATIVITY; no need.. is virtual anyways.. set it up in the constructor 
const int BANKS = 1;
const int NUMBER_CACHE_LINES = 1024; //1024 lines => 10 bits
const int NUMBER_DATA_BLOCKS = 64; //Data Block = 64 bytes => 8 bits
const ReplacementPolicy REPLACEMENT_POLICY = RANDOM ; //random=0

