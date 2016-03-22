/*
	Ducksim: Cache Default values
*/

#ifndef _DEFAULTS_H
#define _DEFAULTS_H	

#include <map>
#include <string>
#include "types.h"



const uint CACHE_SIZE = 65536; //64KB => tag=32-10-8=14
const int ASSOCIATIVITY = 2; //Direct Mapped=1, Fully Associative=0
//const int VIRTUAL_BANKS = ASSOCIATIVITY; no need.. is virtual anyways.. set it up in the constructor 
const int BANKS = 1;
const int NUMBER_CACHE_LINES = 1024; //1024 lines => 10 bits
const int NUMBER_DATA_BLOCKS = 64; //Data Block = 64 bytes => 8 bits
/*
const int REPLACEMENT_POLICY = 1; //random=0

const std::map < std::string, int > r_policies_map = { //Mapping policies to their number index
	{"random", 0},
	{"fifo", 1}
};*/

const ReplacementPolicy REPLACEMENT_POLICY = FIFO ; //random=0

#endif
