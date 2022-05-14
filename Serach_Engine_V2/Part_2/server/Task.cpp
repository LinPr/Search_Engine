#include "Task.h"
#include "json.hpp"
#include "KeyRecommender.h"
#include "Reactor_ThreadPool.h"
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <unistd.h>

void Task::taskProcessing()
{
    std::cout << "Task::taskProcessing()" << std::endl;

    /* processing msg */
    /* 0. 拿到 keyRecommender 对象 */
    KeyRecommender & keyRecommender = _preactor_threadPoll->getKeyRecommender();

    /* 1. 模糊匹配 */
    bool is_matched = keyRecommender.fuzzyMatching(_msg);
    if(is_matched == false)
    { //输入错误
        _pconnection_manager->registerIOtask("[404]: can't find anything, try again\n"); 
        return;
    }

    /* 2. 若正确匹配到，将字符串数组用 json 封包 */
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
    _pconnection_manager->registerIOtask(json_recommendWords.dump()); 
    
}

