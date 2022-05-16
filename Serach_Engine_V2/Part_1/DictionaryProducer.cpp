#include "DictionaryProducer.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>


using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::map;


DictionaryProducer::DictionaryProducer(string directoryPath, string stopWordPath, string storePath) 
: _directoryPath(directoryPath)
, _stopWordPath(stopWordPath)
, _storePath(storePath)
{
    cout << "DictionaryProducer(const std::string & directoryPath)" << endl;

    getFilePaths();
    getStopWords();

}

DictionaryProducer::~DictionaryProducer()
{
    cout << "~DictionaryProducer" << endl;
}

void DictionaryProducer::buildEnDictionary()
{
    map<string, int> dictonary;

    /* 按顺序打开文件路径，读取文件*/
    for(auto & path : _fliePaths)
    {
        ifstream ifs(path);
        string word;
        while(ifs >> word)
        { // 过滤停用词，去除标点符号，数字,空格， 转成小写
            /* 这里有问题 */
            if(_stopWords.size() && (_stopWords.find(word) != _stopWords.end())) 
                { continue; }
                
            string tmp;
            for(auto it = word.begin(); it != word.end(); ++it)
            {
                // if(!ispunct(*it) && !isdigit(*it)) 
                if(isalpha(*it))
                    { tmp += tolower(*it); } 
            }   
            if(_stopWords.find(tmp) != _stopWords.end()) 
                { continue; }
                
            dictonary[tmp]++;
        }
    }

    /* 将map 转换成 vector，以便于后续查询利用vector的完美哈希特性（下标访问）*/
    for(auto e : dictonary)
    {
        _dictionay.push_back(e);
    }

}

void DictionaryProducer::buildEnDictionaryIndex()
{
    if(!_dictionay.size())
    {
        buildEnDictionary();
    }

    for(int line = 0; line < _dictionay.size(); ++line)
    {
        for(auto alphabet : _dictionay[line].first)
        {
            string tmp; tmp += alphabet;
            _dictionaryIndex[tmp].insert(line); 
        }
    }
}

void DictionaryProducer::storeDictionary(string storePath)
{ // 写到文件中持久化存储
    string dictionaryPath = string(storePath) + "/dictionary";
    string dictionaryIndexPath = string(storePath) + "/dictionaryIndex";

    ofstream ofs_1(dictionaryPath);
    for(auto & e : _dictionay)
    {
        ofs_1 << e.first << "\t" << e.second << endl;
    }

    ofstream ofs_2(dictionaryIndexPath);
    for(auto & e : _dictionaryIndex)
    {
        ofs_2 << e.first << "\t";
        
        for(auto & j : e.second)
        {
            ofs_2 << j << "\t" ;
        }
        ofs_2 << endl;
    }
}

void DictionaryProducer::showDictionary() const
{
    for(auto & e : _dictionay)
    {
        cout << e.first << "\t" << e.second << endl;
    }
}

void DictionaryProducer::showStopWords() const
{
    for(auto & e : _stopWords)
    {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

void DictionaryProducer::pushDictionary()
{
    // ??????? what ??

}

void DictionaryProducer::showFilePathes() const
{
    cout << "DictionaryProducer::showFilePathes()" << endl;

    for(auto path : _fliePaths)
    {
        cout << path << endl;
    }
    cout << _stopWordPath << endl;
    cout << _storePath << endl;


}

void DictionaryProducer::getFilePaths()
{   
    //打开目录
    DIR* dir_p = opendir(_directoryPath.c_str());
    struct dirent* dirent_p;
    while ((dirent_p = readdir(dir_p)) != NULL )
    {   
        if(!strcmp(dirent_p->d_name, ".")  || !strcmp(dirent_p->d_name, ".."))
            continue;
        
        string filePath(_directoryPath);
        filePath += "/";
        filePath += dirent_p->d_name;
        _fliePaths.push_back(filePath);
    }
}

void DictionaryProducer::getStopWords()
{
    std::cout << "DictionaryProducer::getStopWords()" << std::endl;
    ifstream ifs(_stopWordPath);
    string stopWord;
    while(ifs >> stopWord)
    {   
        _stopWords.insert(stopWord);
    }
}


