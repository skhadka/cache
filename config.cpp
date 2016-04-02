/*
Parses the config file, creates and returns a Cache object.
*/

#include "config.hpp"
#include <algorithm> 
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "replacement_policies.h"


using namespace Config;

 /*Helper function, used to get the replacement policy. */
static ReplacementPolicy Config::getRP(std::string RP){
    std::map<std::string, ReplacementPolicy> ReplacementPolicyMap;
        ReplacementPolicyMap["RANDOM"]=RANDOM;
        ReplacementPolicyMap["FIFO"]=FIFO;
        ReplacementPolicyMap["LRU"]=LRU;

    ReplacementPolicy policy = ReplacementPolicyMap[RP];
    
    return policy;
}

/*Parses config file, creates and returns a Cache object.*/
Cache Config::configureCache(){ 
    std::map<std::string, std::string> cacheSettings;
    std::string line="",property="", value;
    std::ifstream file;
    file.open("cache.conf");
    
    while(!file.eof()){
        getline(file,line); //Gets one instruction line, consisting of cache property and value.
        line.erase(remove_if(line.begin(), line.end(), (int(*)(int))isspace), line.end());
        std::istringstream ss(line);
        if(line[0]!='\n' && line[0]!='#' && getline(ss,property,'=')){ //Splits line into 2 tokens, property and value.
            if(getline(ss,value)) cacheSettings[property]=value;//Creates a map of cache properties and their corresponding values.
        }
    }

    /*Create cache object as specified by the config file.*/
    Cache cache = Cache(stoi((cacheSettings["CACHE_SIZE"])), stoi((cacheSettings["ASSOCIATIVITY"])), stoi((cacheSettings["BANKS"])), stoi((cacheSettings["NUMBER_CACHE_LINES"])),stoi((cacheSettings["NUMER_DATA_BLOCKS"])), Config::getRP(cacheSettings["REPLACEMENT_POLICY"]));
    return cache;
}