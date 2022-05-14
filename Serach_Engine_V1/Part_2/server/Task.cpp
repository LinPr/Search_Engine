#include "Task.h"
#include "json.hpp"
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <unistd.h>

void Task::taskProcessing()
{
    std::cout << "Task::taskProcessing()" << std::endl;

    /* processing msg */
    
    /* 1. 模糊匹配 */
    bool is_matched = fuzzyMatching();
    if(is_matched == false)
    {
        _pconnection_manager->registerIOtask("can't find anything\n"); 
        return;
    }

    /* 2. 若正确匹配到，将字符串数组用 json 封包 */
    // std::vector<std::string> _recommendWords;
    // std::vector<std::string> recommendWords{"lin", "wang", "zhagn"};
    std::cout << "_recommendWords.size() = " << _recommendWords.size() << std::endl;
    for(auto i : _recommendWords)
    {
        std::cout << i << " " << std::endl;
    }
    std::cout << std::endl;
    
    nlohmann::json json_recommendWords(_recommendWords);


/*将 IO 任务从整个任务中抽离出来，注册到Reactor中的IO任务队列中，等时机合适时再执行*/
    _pconnection_manager->registerIOtask(json_recommendWords.dump()); 
}



/* 模糊匹配算法 */
bool Task::fuzzyMatching()
{
    /* 0. 格式化一下 _msg 去掉没用的信息, 转成小写 */
    std::string word; 
    for(auto it = _msg.begin(); it != _msg.end(); ++it)
    {
        if(isalpha(*it))
            { word += tolower(*it); } 
    }
    if(word.size() == 0) 
        { return false; }
    

    /* 1. 将读入的字符通过词典索引，找到词典中 pretty matching 的字符 */
    std::map<std::string, std::set<int>> & dictionaryIndex = _dictionary.getDictionaryIndex();

    std::string tmp_1;  tmp_1 += word[0]; 
    std::set<int> recommendWordsIndex = dictionaryIndex[tmp_1];
   
    for(auto alphabet : word)
    { // 取出完美匹配词在字典中出现行的交集
        std::set<int> tmpWordsIndex;
        std::string tmp_2;  tmp_2 += alphabet;
        std::set<int> wordsIndex = dictionaryIndex[tmp_2];

        // 特别注意 set_intersection，只能对有序集合取交集
        std::set_intersection ( recommendWordsIndex.begin(), recommendWordsIndex.end(),
                                wordsIndex.begin(), wordsIndex.end(),
                                std::insert_iterator<std::set<int>>(tmpWordsIndex, tmpWordsIndex.begin()));

        recommendWordsIndex.swap(tmpWordsIndex);
        // tmpWordsIndex.clear();
    }

    
    for(auto i : recommendWordsIndex)
    { // 调试查询索引结果（正确）
        std::cout << i+1 << " "; // 在文本文件中显示的行号从 1 开始
    }
    std::cout << std::endl;

    /* 优先级：编辑距离 > 词频 > (字母顺序) */
    // 以编辑距离为索引，利用 map 关于key 的自动排序功能

    std::vector<std::pair<std::string,int>> dictionary = _dictionary.getDictionary();
    
    // 这里最小编辑距离key值可能相同，因此数据结构用 multimap
    std::multimap<int, std::string, std::less_equal<int>> _distance2word_map;
    
    if(recommendWordsIndex.size() >= 5)
    { // 直接计算编辑距离并排序即可
        for(auto line_no : recommendWordsIndex)
        {
            std::string candidateWord = dictionary[line_no].first;
            _distance2word_map.emplace(minEditDistence(word, candidateWord), candidateWord);
        }

        _recommendWords.clear(); // 清空一下缓冲区
        for(auto & i : _distance2word_map)
        {
            _recommendWords.push_back(i.second);
            if(_recommendWords.size() == 5)
            {  // 提前返回
                return true;  
            }
        }
    }
    else if (recommendWordsIndex.size() < 5)
    { /* 考虑如果不足 5 个数的情况 */
        for(auto & line_no : recommendWordsIndex)
        {
            std::string candidateWord = dictionary[line_no].first;
            _recommendWords.push_back(candidateWord);
        }
        
        // 剩下不足个数的推荐词就需要遍历整个词典
        for(int line = 0; line < dictionary.size(); ++line)
        {
            if(recommendWordsIndex.find(line) != recommendWordsIndex.end())
                continue;
            else
            {
                std::string candidateWord = dictionary[line].first;
                _distance2word_map.emplace(minEditDistence(word, candidateWord), candidateWord);
            }
        }

        for(auto & i : _distance2word_map)
        {
            _recommendWords.push_back(i.second);
            if(_recommendWords.size() == 5)
            {
                return true; //提前返回
            }
                
        }
    }
    

    
    /* 其他情况 */
    return false; 

}


int Task::minEditDistence(std::string word1, std::string word2)
{ // 动态规划算法

    int n = word1.length();
    int m = word2.length();

    // 有一个字符串为空串
    if (n * m == 0) return n + m;

    // DP 数组
    std::vector<std::vector<int>> D(n + 1, std::vector<int>(m + 1));

    // 边界状态初始化
    for (int i = 0; i < n + 1; i++) {
        D[i][0] = i;
    }
    for (int j = 0; j < m + 1; j++) {
        D[0][j] = j;
    }

    // 计算所有 DP 值
    for (int i = 1; i < n + 1; i++) {
        for (int j = 1; j < m + 1; j++) {
            int left = D[i - 1][j] + 1;
            int down = D[i][j - 1] + 1;
            int left_down = D[i - 1][j - 1];
            if (word1[i - 1] != word2[j - 1]) left_down += 1;
            D[i][j] = std::min(left, std::min(down, left_down));
        }
    }
    return D[n][m];
};

