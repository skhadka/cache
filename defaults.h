/*
	Ducksim: Cache Default values
*/

#ifndef _DEFAULTS_H
#define _DEFAULTS_H	

#include <map>
#include <string>
#include "types.h"

/*Configuration*/
const uint CACHE_SIZE = 32;//65536; //64KB => tag=32-10-8=14
const int ASSOCIATIVITY = 4; //Direct Mapped=1, Fully Associative=0
const int BANKS = 1;
const int NUMBER_CACHE_LINES = 8;//1024; //1024 lines => 10 bits
const int NUMBER_DATA_BLOCKS = 4;//64; //Data Block = 64 bytes => 8 bits

/*Replacement Policy*/
const ReplacementPolicy REPLACEMENT_POLICY = RANDOM; //random=0

/*Read Policy*/

/*Write Policy*/

#endif
