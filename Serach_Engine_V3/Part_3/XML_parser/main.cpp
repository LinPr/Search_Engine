#include "WebPageParser.h"
#include <unistd.h>

using std::cout;
using std::endl;

int main()
{
    WebPageParser webPageParser("./config.rc");
    
    // webPageParser.showStopWords();
    // webPageParser.showXmlFilePaths();
    webPageParser.startParse();
    

    // std::vector<WebPage> & webPageLib = webPageParser.get_webPageLib();
    // std::map<int, std::pair<int, int>> & pagesOffsetMap = webPageParser.get_pagesOffsetMap();

    // webPageParser.show_webPageLib();
    
    // webPageParser.show_pagesOffsetMap();
    webPageParser.show_invertIndex();


    
    
}