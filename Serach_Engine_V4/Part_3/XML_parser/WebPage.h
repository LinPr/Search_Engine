#pragma once 

#include <string>
#include <vector>
#include <map>

using std::string;

/* 用于存储每一篇已经解析好的网页文档, 并且用xml语法格式化 */
class WebPage
{
public:
    WebPage(int pageID, string pageTitle, string pageURL, string pageFullText);

    std::string & get_xmlPage();
    
    int get_pageID();
    std::string & get_pageTitie();
    std::string & get_pageURL();
    std::string & get_pageFullText();


public:
    std::vector<std::string> _top10Words; // 10 个热词
    std::map<std::string, int> _wordsMap; // 每个词的词频map

private:
    std::string _xmlPage; // 等于后面所有内容相加

    int _pageID;
    std::string _pageTitle;
    std::string _pageURL;
    std::string _pageFullText;
};
