#pragma once
#include "NonCopyable.h"
#include "LRUCache.h"
#include <string>
#include <vector>
#include <unordered_map>

class LRUCachesManager
: NonCopyable
{
public:
    LRUCachesManager(int CacheCapacity);

    void addCache(pthread_t thid);
    
    void updateCachePendign();

public:
    std::unordered_map<pthread_t, LRUCache> _LRUCacheGroup;


private: 
    int _CacheCapacity;
    
};
