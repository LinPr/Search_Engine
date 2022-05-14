#pragma once


#include <string>
#include <vector>
#include <map>
#include <set>

class Dictionary
{
public:
    Dictionary(std::string dictionaryPath, std::string dictionaryIndexPath);
    
    void loadDictionary();
    void showDictionary();

    std::vector<std::pair<std::string,int>> & getDictionary();
    std::map<std::string, std::set<int>> & getDictionaryIndex();

private:
    std::string _dictionaryPath;
    std::string _dictionaryIndexPath;

    std::vector<std::pair<std::string,int>> _dictionay; //替代map使得时间复杂度为 O1
    std::map<std::string, std::set<int>> _dictionaryIndex; //字典的索引行号
};


