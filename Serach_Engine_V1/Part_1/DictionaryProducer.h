#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>

class DictionaryProducer
{
public:
    DictionaryProducer(std::string directoryPath, std::string stopWordPath, std::string storeDictionaryPath); // 处理英文
    ~DictionaryProducer();

    void buildEnDictionary();
    void buildEnDictionaryIndex();
    void storeDictionary(std::string storePath);
    void showDictionary() const;
    void pushDictionary();
    void showFilePathes() const; 

private:
    void getFilePaths();
    void getStopWords();

private:
    std::vector<std::string> _fliePaths; // 语料库文件绝对路径集合
    std::vector<std::pair<std::string,int>> _dictionay; //替代map使得时间复杂度为 O1
    std::map<std::string, std::set<int>> _dictionaryIndex; //字典的索引行号
    // SplitTool & _splitTool;

private:
    std::set<std::string> _stopWords;

    std::string _directoryPath;
    std::string _stopWordPath;
    std::string _storePath; //存储字典其索引的绝对路径
};

