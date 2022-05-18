#include "LRUCache.h"

LRUCache::LRUCache(int capacity)
: _capacity(capacity)
{
    
}


Value LRUCache::getValue(Key key)
{
    if(_key2cacheNode_map.find(key) == _key2cacheNode_map.end())
        return Value();
    
    /* 更新下cache热数据 */
    putKeyValue(key, _key2cacheNode_map[key]->second);
    
    return _key2cacheNode_map[key]->second;
}



void LRUCache::putKeyValue(Key key, Value value)
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



void LRUCache::addKeyValue2PendingList(Key key, Value value)
{
    
}