#include "WebPageParser.h"
#include "tinyxml2.h"
#include <string>
#include <regex>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <cmath>


using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::map;
using std::pair;
using std::make_pair;
using std::regex;
using namespace tinyxml2;
using std::sort;
using std::unordered_map;


WebPageParser::WebPageParser(std::string configFilePath)
: _config(configFilePath)
{
    get_xmlFliePaths();
    get_stopWords();
}

void WebPageParser::startParse()
{
    int pageID = 0;
    int pageStart = 0;
    int pageOffset = 0;

    /* 一、 处理所有的xml file */
    for(auto xmlFile : _xmlFliePaths)
    {
        /* 1. 加载文件 */
        XMLDocument doc;
        doc.LoadFile(xmlFile.c_str());
        if(doc.ErrorID()) { cerr << "doc.LoadFile" << endl; return; }
        
        /* 2. 获取根节点下面的第一个 Item */
        XMLElement * itemNode = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");

        /* 3. 遍历每一个 Item */
        // if(itemNode)
        while(itemNode)
        {   
        
            /* a. 获取节点中的文本信息 */
            string pageTitle = itemNode->FirstChildElement("title")->GetText();
            string pageLink = itemNode->FirstChildElement("link")->GetText();
            string pageURL = itemNode->FirstChildElement("link")->GetText();
            string pageFullText = itemNode->FirstChildElement("full-text")->GetText();


            /* b. 使用正则表达式过滤 */
            string pattern = "<[^>]+>";
            std::regex reg(pattern);
            pageFullText = std::regex_replace(pageFullText, reg, "");


            /* c. 建立网页库 */
            WebPage webPage(pageID, pageTitle, pageURL, pageFullText);
            _webPageLib.emplace_back(webPage);
            string & xmlPage = webPage.get_xmlPage();


            /* d. 建立网页偏移库 */
            ++pageID;
            pageOffset = xmlPage.size();
            _pagesOffsetMap.emplace(pageID,make_pair(pageStart, pageOffset));
            pageStart += xmlPage.size();
            
            
            // // 输出
            // cout << pageTitle << endl;
            // cout << pageLink << endl;
            // cout << pageFullText << endl;


            itemNode = itemNode->NextSiblingElement("item");
        }
    }

    /* 二、统计每篇文章中的词频，并找出热词 */
    build_pageWordMap();


    /* 三、建立倒排索引库  */
    build_invertIndex();


    /* 四、存储生成的三个文件 */
    storeParsedPages();

}

void WebPageParser::build_pageWordMap()
{
    
    for(auto & webPage : _webPageLib) //这里一定要用引用
    {
        
        // cout << endl << webPage.get_pageID() << endl;
        
        istringstream fullText(webPage.get_pageFullText());
        string word;

        while(fullText >> word)
        {   /* 1. 对文本内容进行过滤（标点，数字，停用词） */
            if(_stopWords.size() && _stopWords.find(word) != _stopWords.end())
                { continue; }

            string tmp; 
            for(auto it = word.begin(); it != word.end(); ++it)
            {
                if(isalpha(*it))
                    { tmp += tolower(*it); }
            }

            if(_stopWords.find(tmp) != _stopWords.end() || tmp.size() == 0)
                { continue; }

            /* 2. 统计每篇文章的词频，注意webPage一定要用引用，直接对原对象修改 */
            webPage._wordsMap[tmp]++;
            
        }

        /* 3. 统计该文章中出现词频最高的10个单词 */
        vector<pair<int, string>> wordsMap2Vector;
        for(auto & w : webPage._wordsMap)
        {
            wordsMap2Vector.emplace_back(make_pair(w.second, w.first));
        }
        /* 使用lambda表达式排序pair类型 */
        sort(wordsMap2Vector.begin(), wordsMap2Vector.end(), 
            [](pair<int, string> & a, pair<int, string> & b){return a.first > b.first;});

        // for(auto i : wordsMap2Vector)
        // {
        //     cout << i.first << " " << i.second << endl;
        // }

        for(int i = 0; i < 10 && i < wordsMap2Vector.size(); ++i)
        {
            webPage._top10Words.emplace_back(wordsMap2Vector[i].second);
            // cout << wordsMap2Vector[i].second << endl;
        } 
    }
}


void WebPageParser::build_invertIndex()
{  
    cout << "WebPageParser::build_invertIndex()" << endl;

    for(auto & webPage : _webPageLib)
    { // 逐篇文章处理

        unordered_map<string, double> singlePage_wordsStrength; 

        for(auto & word : webPage._wordsMap)
        {
            /* 1. TF 单词在一篇文章中出现的次数 */
            int wordFrequence = word.second;

            /* 2. DF 含有该单词的文章篇数 */
            int pageFrequence = 0;
            for(auto & page : _webPageLib)
            {  // 如果在某文章中出现
                if( page._wordsMap.find(word.first) != page._wordsMap.end() )
                {
                    ++ pageFrequence;
                }
            }

            /* 3. 文章总数 */
            int pagesNum = _webPageLib.size();

            /* 4. 计算单词wordStrength =  TF * IDF = TF * log2(N/(DF+1)) */
            double wordStrength =  wordFrequence * log2(pagesNum/(pageFrequence + 1.0));

            
            singlePage_wordsStrength.emplace(make_pair(word.first, wordStrength));
        }

        /* 4. 对一篇文章中的所有单词权重进行归一化处理 */
        double totalStrength = 0;
        for(auto & i : singlePage_wordsStrength)
        {
            totalStrength += i.second * i.second;
        }
        totalStrength = sqrt(totalStrength);
        for(auto & i : singlePage_wordsStrength) 
        { // 这里一定要用引用直接对原数据进行修改
            i.second = i.second / totalStrength;
        }


        /* 5. 将其加入到倒排索引表中 */
        for(auto & i : singlePage_wordsStrength)
        {   
            /* 查看索引表是否已经有该单词 */
            // auto it = _invertIndex.find(i.first);
            // if(it != _invertIndex.end())
            // {
            //     it->second.emplace_back(webPage.get_pageID(), i.second);
            // }
            // else
            // {
            //     _invertIndex[i.first].emplace_back(webPage.get_pageID(), i.second);
            // }
            
            _invertIndex[i.first].emplace_back(make_pair(webPage.get_pageID(), i.second));
        }

    cout << webPage.get_pageID() << "\t" ; // 用于表示正在执行
    fflush(stdout);
    } //end of for(auto & webPage : _webPageLib)

    cout << endl;
}



void WebPageParser::show_xmlFliePaths()
{
    cout << "WebPageParser::show_xmlFliePaths" << endl;
    for(auto & path : _xmlFliePaths)
    {
        cout << path << endl;
    }
}

void WebPageParser::show_stopWords()
{
    cout << "WebPageParser::showStopWords()" << endl;
    for(auto i : _stopWords)
    {
        cout << i << " ";
    }
    cout << endl;
}



void WebPageParser::show_webPageLib()
{
    /* 打印网页内容 */
    for(auto & page : _webPageLib)
    {
        cout << page.get_pageID() << endl;
        cout << page.get_pageTitie() << endl;
        cout << page.get_pageFullText() << endl;
    }


}

void WebPageParser::show_pagesOffsetMap()
{
    /* 打印offset */
    for(auto offMap : _pagesOffsetMap)
    {
        cout << offMap.first << ": ";
        cout << offMap.second.first << " " << offMap.second.second ;
        cout << endl;
    }
}

void WebPageParser::show_invertIndex()
{
    for(auto & invertIdx : _invertIndex)
    {
        cout << invertIdx.first << ":\t" ;
        for(auto & i : invertIdx.second)
        {
            cout << "{" << i.first << ", " << i.second << "}\t";
        }
        cout << endl;
    }
}


std::vector<WebPage> & WebPageParser::get_webPageLib()
{
    return _webPageLib;
}

std::map<int, std::pair<int, int>> & WebPageParser::get_pagesOffsetMap()
{
    return _pagesOffsetMap;
}

void WebPageParser::get_xmlFliePaths()
{
    //打开目录
    DIR* dir_p = opendir(_config.get_xmlDirectoryPath().c_str());
    struct dirent* dirent_p;
    while ((dirent_p = readdir(dir_p)) != NULL )
    {   
        if(!strcmp(dirent_p->d_name, ".")  || !strcmp(dirent_p->d_name, ".."))
            continue;
        
        string filePath(_config.get_xmlDirectoryPath());
        filePath += "/";
        filePath += dirent_p->d_name;
        _xmlFliePaths.push_back(filePath);
    }
}

void WebPageParser::get_stopWords()
{
    ifstream ifs(_config.get_stopWordsPath());
    string stopWord;
    while(ifs >> stopWord)
    {
        _stopWords.insert(stopWord);
    }
}


void WebPageParser::storeParsedPages()
{
    string webPageLibPath = _config.get_parsedPagesPath() + "/webPageLib.xml";
    string pagesOffsetMapPath = _config.get_parsedPagesPath() + "/pagesOffsetMap";
    string invertIndexPath = _config.get_parsedPagesPath() + "/invertIndex";

    /* 1. 存储 _webPageLib */
    ofstream ofs_webPageLib(webPageLibPath);
        /* 打印网页内容 */
    for(auto & page : _webPageLib)
    {
        ofs_webPageLib << page.get_xmlPage() << endl;
        break; // 暂时只存储一篇内容，用于查看效果
    }


    /* 2. 存储 _pagesOffsetMap */
    ofstream ofs_pagesOffsetMap(pagesOffsetMapPath);
    for(auto offMap : _pagesOffsetMap)
    {
        ofs_pagesOffsetMap << offMap.first << "\t";
        ofs_pagesOffsetMap << offMap.second.first << " " << offMap.second.second ;
        ofs_pagesOffsetMap << endl;
    }


    /* 3. 存储 _invertIndex */
    ofstream ofs_invertIndex(invertIndexPath);
    for(auto & invertIdx : _invertIndex)
    {
        ofs_invertIndex << invertIdx.first << "\t " ;
        for(auto & i : invertIdx.second)
        {
            ofs_invertIndex << i.first << " " << i.second << "\t";
        }
        ofs_invertIndex << endl;
    }
}
