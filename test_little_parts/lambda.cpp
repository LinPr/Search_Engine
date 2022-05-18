#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;


int main()
{
    vector<pair<int, string>> vec;
    vec.push_back({2, "2"});
    vec.push_back({8, "8"});
    vec.push_back({9, "9"});
    vec.push_back({4, "4"});
    vec.push_back({1, "1"});
    vec.push_back({4, "5"});
    vec.push_back({6, "6"});
    
    
    sort(vec.begin(), vec.end(), 
        [](pair<int, string> & a, pair<int, string> & b){return a.first > b.first;});
    for(auto i : vec)
    {
        cout << i.first << i.second << endl;
    }
}