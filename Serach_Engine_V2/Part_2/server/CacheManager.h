#pragma once
#include "NonCopyable.h"
#include "LRUCache.h"
#include <string>
#include <vector>

class CacheManager
: NonCopyable
{
    using LRUCache = LRUCache<std::string, std::vector<std::string>>;

public:
    CacheManager();
    ~CacheManager();

    void initCache();
    LRUCache & getCache();
    void updateCache();

private:
    std::vector<LRUCache> _LRUCacheGroup;
};
