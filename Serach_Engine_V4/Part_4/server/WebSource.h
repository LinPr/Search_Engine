#pragma once

#include "WebPage.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>


class WebSource
{
    using pageID = int; 
    using pageStart = int;
    using pageOffset = int; 
    using wordStrength = double;

public:
    WebSource(std::string webPageLibPath, std::string pagesOffsetMapPath, std::string invertIndexPath);

public:

    std::vector<WebPage> & get_webPageLib();
    std::map<pageID, std::pair<pageStart, pageOffset>> & get_pagesOffsetMap();
    std::unordered_map<std::string, std::vector<std::pair<pageID, wordStrength>>> & get_invertIndex();


    void show_webPageLib();
    void show_pagesOffsetMap();
    void show_invertIndex();
        
private:
    void load_webPageLib(std::string webPageLibPath);
    void load_pagesOffsetMap(std::string pagesOffsetMapPath);
    void load_invertIndex(std::string invertIndexPath);

private:
    std::vector<WebPage> _webPageLib;
    std::map<pageID, std::pair<pageStart, pageOffset>> _pagesOffsetMap;
    std::unordered_map<std::string, std::vector<std::pair<pageID, wordStrength>>> _invertIndex;
};


