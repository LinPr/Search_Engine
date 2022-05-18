#include "LRUCachesManager.h"

LRUCachesManager::LRUCachesManager(int CacheCapacity)
: _CacheCapacity(CacheCapacity)
{
    
}


void LRUCachesManager::addCache(pthread_t thid)
{   
    LRUCache cache(_CacheCapacity);
    _LRUCacheGroup.emplace(thid, cache);
}
