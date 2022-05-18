#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "unistd.h"

using namespace std;

void test_1()
{
       /* 5. 在invertIndex中找出关键词所在文章的交集 */
    // auto & invertIndex = _preactor_threadPoll->get_webSource().get_invertIndex();
    unordered_map<string, vector<pair<int, double>>> invertIndex = {
        { "hello", {{3, 0.4}, {5, 0.4}, {1, 0.1}, {2, 0.2}} }, 
        { "world", {{3, 0.3}, {4, 0.4}, {1, 0.2}, {2, 0.2}} },
        { "fucku", {{1, 0.2}, {2, 0.2}, {4, 0.3}, {4, 0.4}} },
        { "bitch", {{1, 0.1}, {2, 0.2}, {3, 0.3}, {4, 0.4}} }
    };

    vector<string> wordVec = {"hello", "world"};
    std::vector<std::vector<std::pair<int, double>>> resultWords_page2stregth;
    
    for(auto wd : wordVec)
    {
        std::vector<std::pair<int, double>> tmpVec;
        auto it = invertIndex.find(wd);
        if(it == invertIndex.end()) { return; }
        else { tmpVec = it->second;}
        /* 使用lambda表达式对pair进行排序 */
        std::sort(tmpVec.begin(),
         tmpVec.end(), 
            [](std::pair<int, double>& a , std::pair<int, double> b){ return a.first < b.first;});

        resultWords_page2stregth.emplace_back(tmpVec);
    }
#if 1
    for(auto i : resultWords_page2stregth)
    {
        for(auto j : i)
        {
            cout << "{" << j.first << ", " << j.second << "} ";
        }
        cout << endl;
    }
#endif

    cout << endl;
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

        for(auto i : wordInvertIndexVec)
        {

            std::cout << "{" << i.first << ": " << i.second << "} " ;
        }
        std::cout << std::endl;
    }

    #if 0    /* 显示测试一下取完交集的数据 */
        for(auto i : wordInvertIndexVec)
        {

            std::cout << "{" << i.first << ": " << i.second << "} " ;
        }
        std::cout << std::endl;
    #endif
}

bool comp(pair<int, int> &a, pair<int, int> &b )
{
    return a.first < b.first; 
}
void test_2()
{

    vector<pair<int, int>> v1 = {
        {1, 1}, {2, 1}, {3, 2}, {2, 2}, {1, 3}
    };
    vector<pair<int, int>> v2 = {
        {1, 1}, {2, 3}, {3, 2}, {4, 2}, {1, 3}
    };
    vector<pair<int, int>> res;
    
    // v1.push_back(make_pair(1,1)); 
    // v1.push_back(make_pair(2,1));
    // v1.push_back(make_pair(3,2)); 
    // v1.push_back(make_pair(2,2));
    // v1.push_back(make_pair(1,3)); 
    
    
    // v2.push_back(make_pair(1,1)); //same
    // v2.push_back(make_pair(2,3));
    // v2.push_back(make_pair(3,2)); //same
    // v2.push_back(make_pair(4,2));
    // v2.push_back(make_pair(1,3)); //same
    
    sort(v1.begin(), v1.end(), comp);
    sort(v2.begin(), v2.end(), comp);
    
    set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), inserter(res, res.begin()), comp);
    
    cout << "Intersection : " << endl;
    for(auto it = 0; it < res.size(); it++)
        cout << res[it].first << " " << res[it].second << endl;

}



int main()
{
    // test_1();
    test_2();
}
// {
//     while (first1 != last1 && first2 != last2) 
//     {
//         if (*first1 < *first2) 
//         {
//             ++first1;
//         } else  
//         {
//             if (!(*first2 < *first1)) 
//             {
//                 *d_first++ = *first1++; // *first1 and *first2 are equivalent.
//             }
//             ++first2;
//         }
//     }
//     return d_first;
// }