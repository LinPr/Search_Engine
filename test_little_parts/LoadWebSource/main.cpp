#include "WebSource.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string webPageLibPath = "./ParsedPages/webPageLib.xml";
    string pagesOffsetMapPath = "./ParsedPages/pagesOffsetMap";
    string invertIndexPath = "./ParsedPages/invertIndex";


    WebSource webSource(webPageLibPath, pagesOffsetMapPath, invertIndexPath);
    // webSource.show_webPageLib();
    // webSource.show_pagesOffsetMap();
    webSource.show_invertIndex();

}