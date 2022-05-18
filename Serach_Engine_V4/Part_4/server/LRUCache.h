#pragma once 

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <string>

using Key = std::string;
using Value = std::vector<std::string>;


class LRUCache
{
    using Iterator = std::list<std::pair<Key,Value>>::iterator;
    
public:
    LRUCache(int capacity);

    Value getValue(Key key);
    void putKeyValue(Key key, Value value);
    void addKeyValue2PendingList(Key key, Value value);

private:
    int _capacity;
    std::list<std::pair<Key,Value>> _cacheList;  //也是key-value形式
    std::unordered_map<Key, Iterator> _key2cacheNode_map;

    /* 一个最近更新的数据 pending list */
    std::list<std::pair<Key,Value>> _pendingList;
};



