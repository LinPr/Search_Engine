#include "Task.h"
#include "json.hpp"
#include "KeyRecommender.h"
#include "Reactor_ThreadPool.h"
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)


void Task::taskProcessing()
{
    std::cout << "Task::taskProcessing() : therad_id = " << gettid() << std::endl;

    /* processing msg */
    /* 0. 拿到 keyRecommender 对象 */
    KeyRecommender & keyRecommender = _preactor_threadPoll->getKeyRecommender();

    /* 1. 首先去缓存中进行匹配 */
    bool is_matched = keyRecommender.cacheMatching(_msg);
    std::string where_matched;
    if(is_matched == true)
    {
        where_matched = "[200]: matched in cache\n";
    }
    else
    {
        /* 2. 去词典库中模糊匹配 */
        is_matched = keyRecommender.fuzzyMatching(_msg);
        if(is_matched == true)
        { 
            where_matched = "[200]: matched in dictionary\n";      
        }
        else
        { //输入错误
            where_matched = "[404]: can't find anything, try again\n"; 
            return;
        }
    }


    /* 3. 若正确匹配到，将字符串数组用 json 封包 */
    auto recommendWords = keyRecommender.getRecommendWords();
    std::cout << "recommendWords.size() = " << recommendWords.size() << std::endl;
    for(auto i : recommendWords)
    {
        std::cout << i << " " << std::endl;
    }
    std::cout << std::endl;

    // std::vector<std::string> recommendWords{"lin", "wang", "zhagn"}
    nlohmann::json json_recommendWords(recommendWords);
/*将 IO 任务从整个任务中抽离出来，注册到Reactor中的IO任务队列中，等时机合适时再执行*/
    std::string thread_info = "therad_id = " + std::to_string(gettid()) + "\n";
    std::string information = thread_info + where_matched + json_recommendWords.dump() +"\n";
    // 这里有bug，重复注册回调函数有问题，可能是因为const的缘故
    _pconnection_manager->registerIOtask(information); 

}

