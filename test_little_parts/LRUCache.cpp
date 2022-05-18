#include <iostream>
#include <unordered_map>
#include <list>

using std::cout;
using std::endl;

template <typename Key, typename Value>
class LRUCache
{
    using Iterator = std::list<std::pair<int,int>>::iterator;
public:
    LRUCache(int capacity);

    Value get(Key key);
    void put(Key key, Value value);
private:
    int _capacity;
    std::list<std::pair<Key,Value>> _cacheList;  //也是key-value形式
    std::unordered_map<Key, Iterator> _key2cacheNode_map;
    
};


template <typename Key, typename Value>
LRUCache<Key, Value>::LRUCache(int capacity)
: _capacity(capacity)
{
    
}

template <typename Key, typename Value>
Value LRUCache<Key, Value>::get(Key key)
{
    if(_key2cacheNode_map.find(key) == _key2cacheNode_map.end())
        return -1;
    
    /* 更新下cache热数据 */
    put(key, _key2cacheNode_map[key]->second);
    
    return _key2cacheNode_map[key]->second;
}


template <typename Key, typename Value>
void LRUCache<Key, Value>::put(Key key, Value value)
{   
    auto it = _key2cacheNode_map.find(key);
    if( it != _key2cacheNode_map.end())
    { // 如果新加入加点已经在cache中存在

        /* 删除老元素 */
        _cacheList.erase(it->second); //删除链表中节点
        _key2cacheNode_map.erase(it); // 删除哈希表对应行

        /* 添加新元素 */
        _cacheList.push_front({key, value});
        _key2cacheNode_map[key] = _cacheList.begin(); 
    }
    else
    { // 如果新加入节点在cache中不存在
        if(_cacheList.size() == _capacity)
        { 
            /* 删除老元素 */
            _key2cacheNode_map.erase( _cacheList.back().first);
            _cacheList.pop_back(); // 删除cacheList最后一个元素
        }

        /* 添加新元素 */
        _cacheList.push_front({key, value});
        _key2cacheNode_map[key] = _cacheList.begin();
    }
}



int main()
{

    LRUCache<int, int> cache(2);
    cache.put(1, 1); // 缓存是 {1=1} 
    cache.put(2, 2); // 缓存是 {1=1, 2=2}
    cout << cache.get(1) << endl;   // 返回 1
    cache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
    cout << cache.get(2) << endl;    // 返回 -1 (未找到)
    cache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
    cout << cache.get(1) << endl;    // 返回 -1 (未找到)
    cout << cache.get(3)<< endl;    // 返回 3
    cout << cache.get(4) << endl;    // 返回 4

}