#include "LRUCache.h"
#include "LRUCachesManager.h"
#include "pthread.h"
#include "unistd.h"

using namespace std;


void* func(void * arg)
{
    while(1)
    {
        // cout << 111111111111111 << endl;
        sleep(1);
    }
    
}

int main()
{
    LRUCachesManager cachesManager(10);

    pthread_t thid;
    pthread_create(&thid, nullptr, func, nullptr);

    cachesManager.addCache(thid);

    cout << cachesManager._LRUCacheGroup.size() << endl;


    /* 操他妈逼，一定要对map谨慎使用下标运算 */
    auto cache = cachesManager._LRUCacheGroup.at(thid);
    // auto cache = cachesManager._LRUCacheGroup[thid];


    string key = "3";
    vector<string> value = {"33333", "333333", "33333"};




    cache.putKeyValue(key, value);
    
    auto v = cache.getValue(key);
    cout << v.size() << endl;
    for(auto i : v)
    {
        cout << i << endl;
    }
    fflush(stdout);

    while(1);

}