#include <iostream>
#include <unordered_map>
#include <list>

using std::list;
using std::unordered_map;
using std::pair;


class LRUCache {
public:
    LRUCache(int capacity)
    {
        cap = capacity;
    }
    
    int get(int key)
    {
        if (hashMap.count(key) == 0) {
            return -1;
        }

        int value = hashMap[key]->second;
        put(key, value);

        return value;
    }
    
    void put(int key, int value)
    {
        if (hashMap.count(key) > 0) {
            cache.erase(hashMap[key]);
        } else {
            if (cap == cache.size()) {
                int removeKey = cache.back().first;
                hashMap.erase(removeKey);
                cache.pop_back();
            }
        }

        cache.push_front({key, value});
        hashMap[key] = cache.begin();
    }
private:
    int cap;
    unordered_map<int, list<pair<int, int>>::iterator> hashMap;
    list<pair<int, int>> cache;
};