#include "Task.h"
#include "json.hpp"
#include "cosSimilarity.h"
#include "KeyRecommender.h"
#include "Reactor_ThreadPool.h"
#include "WebSource.h"
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

/* 关键词推荐任务 */
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
        where_matched = "[100]: matched in cache\n";
    }
    else
    {
        /* 2. 去词典库中模糊匹配 */
        is_matched = keyRecommender.fuzzyMatching(_msg);
        if(is_matched == true)
        { 
            where_matched = "[100]: matched in dictionary\n";      
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


/* 网页搜索任务 */
void SearchWebPageTask::SearchWebPageTaskProcessing()
{   

    
    // std::string str = "[200]: hello this is SearchWebPageTask\n";

    /* 1. 将输入的单词用 TF-IDF 解析成一个 Base向量 */
  
    std::vector<std::string> wordVec;
    std::vector<double> strengthBaseVec;
    std::istringstream iss(_msg);
    std::string word;
    while(iss >> word)
    {
    /* 1. 对文本内容进行过滤（标点，数字，转小写） */
        
        string tmp; 
        for(auto it = word.begin(); it != word.end(); ++it)
        {
            if(isalpha(*it))
                { tmp += tolower(*it); }
        }

        wordVec.push_back(tmp);
    }

    if(wordVec.size() == 0)
    { // 输入有问题，直接提前返回
        return;
    }

    for(auto wd : wordVec)
    {
        /* 1. 单词在该文章（用户输入）中出现的次数 */
        int wordFrequency = 1; // 这里为了方便直接写 1
        
        /* 2. 单词在文章中出现的篇数, 这里比较巧妙，直接查倒排索引表 */
        auto invertIndex = _preactor_threadPoll->get_webSource().get_invertIndex();
        auto pageNum2strengthVec = invertIndex.find(wd);
        if(pageNum2strengthVec == invertIndex.end())
        {
            //提前返回
            _pconnection_manager->registerIOtask("[404]: can't match all words\n");
            return;
        }
        int pageFrequency = pageNum2strengthVec->second.size();
        /* int pageFrequency = invertIndex.at(wd).size(); //错误写法 */
    

        /* 3. 文章总数 */
        int pageNum = _preactor_threadPoll->get_webSource().get_webPageLib().size();


        /* 4. 计算每个单词wordStrength */
        double wordStregth = wordFrequency * log2(pageNum/(pageFrequency) + 1.0);

        strengthBaseVec.push_back(wordStregth);
    }


    /* 4. 对权重进行归一化处理 */
    double totalStrength = 0;
    for(auto & i : strengthBaseVec) 
    {
        totalStrength += i * i;
    }
    totalStrength = sqrt(totalStrength);
    for(auto & i : strengthBaseVec) //这里一定要用引用
    {
        i = i / totalStrength; 

    }

#if 0 /* 测试 */
    string str;
    for(auto i : wordVec) { str += (i + " "); }
    str += "\n";
    for(auto i : strengthBaseVec) { str += (std::to_string(i) + " "); }
    str += "\n";
    _pconnection_manager->registerIOtask(str);
#endif


    /* 5. 在invertIndex中找出关键词所在文章的交集 */
    auto & invertIndex = _preactor_threadPoll->get_webSource().get_invertIndex();
    std::vector<std::vector<std::pair<int, double>>> resultWords_page2stregth;
    
    for(auto wd : wordVec)
    {
        std::vector<std::pair<int, double>> tmpVec;
        auto it = invertIndex.find(wd);
        if(it == invertIndex.end()) { return; }
        else { tmpVec = it->second;}
        /* 使用lambda表达式对pair进行排序 */
        std::sort(tmpVec.begin(), tmpVec.end(), 
            [](std::pair<int, double>& a , std::pair<int, double> b){ return a.first < b.first;});

        resultWords_page2stregth.emplace_back(tmpVec);
    }

    
    
    std::vector<std::pair<int, double>> wordInvertIndexVec = resultWords_page2stregth[0];

    for(auto & i : resultWords_page2stregth)
    {   
        std::vector<std::pair<int, double>> tmpIntersectionVec;

        /* 取交集只能对有序容器进行操作 */
        std::set_intersection(
            i.begin(),i.end()
            , wordInvertIndexVec.begin(), wordInvertIndexVec.end()
            , std::insert_iterator<std::vector<std::pair<int, double>>>(tmpIntersectionVec, tmpIntersectionVec.begin())
            , [](std::pair<int, double> & a, std::pair<int, double> & b){ return a.first < b.first; }
        );
        wordInvertIndexVec.swap(tmpIntersectionVec);
    }

#if 1    /* 显示测试一下取完交集的数据 (结果基本正确)*/
    for(auto i : wordInvertIndexVec)
    {
        std::cout << "{" << i.first << ": " << i.second << "} " ;
    }
    std::cout << std::endl;
#endif

#if 1
    // 计算余弦相似度
    std::vector<std::pair<double, int>> cosSim2pageID;

    for(auto i : wordInvertIndexVec)
    {   
        std::vector<double> simVec;

        /* 1. 按顺序取出共同出现的文章号 */
        int pageID = i.first; 

        /* 2. 构建该文章中所匹配单词的权重向量 */
        for(auto page2strengthVec : resultWords_page2stregth)
        {
            for(auto k : page2strengthVec)
            {
                if(k.first == pageID) 
                {
                    simVec.push_back(k.second);
                    break;
                } 
            }
        }

        // std::cout << "[" ;
        // for(auto i : simVec) { std::cout << i << " " ;}
        // std::cout << "] " << std::endl;

        /* 3. 计算余弦相似度 */
        double cosSim = cosSimilarity(simVec, strengthBaseVec);

        cosSim2pageID.emplace_back(cosSim, pageID);
    }


    /* 4. 根据文章和查询词的余弦相似度对文章进行排序 */
    std::sort(cosSim2pageID.begin(), cosSim2pageID.end(),
    [](std::pair<double, int> & a, std::pair<double, int> & b){return a.first > b.first; });

    for(auto & i : cosSim2pageID)
    {
        printf("{%.16lf, %d}\n", i.first, i.second);
        // std::cout << "{" << i.first << ", " << i.second << "}" << std::endl;
    }
#endif
    
    // 取出单词所在的文章，发给客户端
    std::string resultPages;
    auto & webPageLib = _preactor_threadPoll->get_webSource().get_webPageLib();
    for(auto & i : cosSim2pageID)
    {
         resultPages += (webPageLib[i.second].get_xmlPage() + "\n\n");
    }
    
    _pconnection_manager->registerIOtask(resultPages);
}

