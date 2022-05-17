#include "Dictionary.h"

#include <iostream>
#include <fstream>
#include <sstream>

Dictionary::Dictionary(std::string dictionaryPath, std::string dictionaryIndexPath)
: _dictionaryPath(dictionaryPath)
, _dictionaryIndexPath(dictionaryIndexPath)
{

}


void Dictionary::loadDictionary()
{
    std::cout << _dictionaryPath << std::endl;
    std::cout << _dictionaryIndexPath << std::endl;

    std::string line;
    std::string word;
    int number;
    
    // 读取字典文件
    std::ifstream ifs_1(_dictionaryPath);
    while(getline(ifs_1, line))
    {
        std::istringstream iss(line);
        iss >> word >> number;
        _dictionay.emplace_back(word, number);
    }

    // 读取字典索引
    std::ifstream ifs_2(_dictionaryIndexPath);
    while(getline(ifs_2, line))
    {
        std::istringstream iss(line);
        std::set<int> tmp;
        iss >> word;
        while(iss >> number)
        {
            tmp.insert(number);
        }
        _dictionaryIndex.emplace(word, tmp);
    }

}



void Dictionary::showDictionary()
{
    for(auto & e : _dictionay)
    {
        std::cout << e.first << "\t" << e.second << std::endl;
    }

    for(auto & e : _dictionaryIndex)
    {
        std::cout << e.first << " ";
        for(auto & j : e.second)
        {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}



std::vector<std::pair<std::string,int>> & Dictionary::getDictionary() 
{
    return _dictionay;
}

std::map<std::string, std::set<int>> & Dictionary::getDictionaryIndex()
{
    return _dictionaryIndex;
}