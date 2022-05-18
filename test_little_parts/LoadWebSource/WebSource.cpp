#include "WebSource.h"
#include <iostream>
#include <fstream>
#include <sstream>

using pageID = int; 
using pageStart = int;
using pageOffset = int; 
using wordStrength = double;
using std::cout;
using std::endl;


WebSource::WebSource(std::string webPageLibPath, std::string pagesOffsetMapPath, std::string invertIndexPath)
{
    load_webPageLib(webPageLibPath);
    load_pagesOffsetMap(pagesOffsetMapPath);
    load_invertIndex(invertIndexPath);
}


void WebSource::load_webPageLib(std::string webPageLibPath)
{
    std::cout << "WebSource::load_webPageLib(std::string webPageLibPath)" << endl;
    std::ifstream ifs(webPageLibPath);
    std::string xmlPage;
    
    while(getline(ifs, xmlPage))
    {
        /* 利用数组下标作为网页 ID */
        WebPage webPage(xmlPage);
        _webPageLib.emplace_back(webPage);
    }

}

void WebSource::load_pagesOffsetMap(std::string pagesOffsetMapPath)
{
    std::ifstream ifs(pagesOffsetMapPath);
    std::string line;
    while(getline(ifs, line))
    {   
        std::istringstream iss(line);
        pageID page_id; 
        pageStart page_start;
        pageOffset page_offset; 

        iss >> page_id >> page_start >> page_offset;
        _pagesOffsetMap.emplace(page_id, std::make_pair(page_start, page_offset));
    }
}   

void WebSource::load_invertIndex(std::string invertIndexPath)
{
    std::ifstream ifs(invertIndexPath);
    std::string line;
    while(getline(ifs, line))
    {   
        std::istringstream iss(line);
        std::string word; 
        pageID page_id; 
        wordStrength word_strength;
        iss >> word;
        std::vector<std::pair<pageID, wordStrength>> tmpVector;
        while(iss >> page_id >> word_strength)
        {
            tmpVector.emplace_back(page_id, word_strength);
        }
        _invertIndex.emplace(word, tmpVector);
    }
}



void WebSource::show_webPageLib()
{
    /* 打印网页内容 */
    for(auto & page : _webPageLib)
    {
        cout << page.get_xmlPage() << endl;
    }
}

void WebSource::show_pagesOffsetMap()
{
    /* 打印offset */
    for(auto offMap : _pagesOffsetMap)
    {
        cout << offMap.first << ": ";
        cout << offMap.second.first << " " << offMap.second.second ;
        cout << endl;
    }
}

void WebSource::show_invertIndex()
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
