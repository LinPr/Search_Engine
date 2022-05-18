#include "LRUCachesManager.h"

LRUCachesManager::LRUCachesManager(int CacheCapacity)
: _CacheCapacity(CacheCapacity)
{
    
}


void LRUCachesManager::addCache(pthread_t thid)
{   
    std::cout << "addCache(pthread_t thid) = " << thid << std::endl; 
    LRUCache cache(_CacheCapacity);
    _LRUCacheGroup.emplace(thid, cache);
}

LRUCache & LRUCachesManager::getCache(pthread_t thid)
{
    return _LRUCacheGroup.at(thid);
}

