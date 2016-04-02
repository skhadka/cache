#ifndef _CONFIG_H
#define _CONFIG_H

#include <map>	
#include "types.h"
#include "cache.h"

namespace Config
{		
	Cache configureCache();
	static ReplacementPolicy getRP(std::string rp);
}

#endif