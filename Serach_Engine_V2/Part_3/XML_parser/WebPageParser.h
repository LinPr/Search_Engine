#pragma once

#include "Config.h"
#include "WebPage.h"
#include <string>
#include <set>
#include <map>
#include <unordered_map>

class WebPageParser
{
    using pageID = int; 
    using pageStart = int;
    using pageOffset = int; 
    using wordStrength = double;
public:
    WebPageParser(std::string configFilePath);

private:
    void build_pageWordMap();
    void build_invertIndex();

public:
    void startParse();

    void show_xmlFliePaths();
    void show_stopWords();

    void show_webPageLib();
    void show_pagesOffsetMap();
    void show_invertIndex();

    std::vector<WebPage> & get_webPageLib();
    std::map<pageID, std::pair<pageStart, pageOffset>> & get_pagesOffsetMap();

private:
    

    void get_xmlFliePaths();
    void get_stopWords();
    void storeParsedPages();


private:
    std::vector<std::string> _xmlFliePaths; // 语料库文件绝对路径集合
    std::set<std::string> _stopWords;


private:
    Config _config;
    std::vector<WebPage> _webPageLib;
    std::map<pageID, std::pair<pageStart, pageOffset>> _pagesOffsetMap;
    std::unordered_map<string, std::vector<std::pair<pageID, wordStrength>>> _invertIndex;
};

