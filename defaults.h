/*
	Ducksim: Cache Default values
*/

#ifndef _DEFAULTS_H
#define _DEFAULTS_H	

#include <map>
#include <string>
#include "types.h"

/*Configuration*/
const uint CACHE_SIZE = 65536; //64KB => tag=32-10-8=14
const int ASSOCIATIVITY = 8; //Direct Mapped=1, Fully Associative=0
const int BANKS = 1;
const int NUMBER_CACHE_LINES = 1024; //1024 lines => 10 bits
const int NUMBER_DATA_BLOCKS = 1024; //Data Block = 64 bytes => 8 bits

/*Replacement Policy*/
const ReplacementPolicy REPLACEMENT_POLICY = LRU; //random=0

/*Read Policy*/

/*Write Policy*/

#endif
