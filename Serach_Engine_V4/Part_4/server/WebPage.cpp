#include "WebPage.h"
#include <sstream>


WebPage::WebPage(std::string xmlPage)
: _xmlPage(xmlPage)
{

}

WebPage::WebPage(int pageID, string pageTitle, string pageURL, string pageFullText)
: _pageID(pageID)
, _pageTitle(pageTitle)
, _pageURL(pageURL)
, _pageFullText(pageFullText)
{
    /* 将上述信息综合生成 page */
    std::ostringstream ossPage;
    ossPage << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    ossPage << "<doc>";
    ossPage << "<docid>" << _pageID << "</docid>";
    ossPage << "<url>" << _pageURL << "</url>";
    ossPage << "<title>" << _pageTitle << "</title>";
    ossPage << "<full-text>" << "<![CDATA[" << _pageFullText <<  "]]>" << "</full-text>";
    ossPage << "</doc>";

    _xmlPage = ossPage.str();
}


std::string & WebPage::get_xmlPage()
{
    return _xmlPage;
}

int WebPage::get_pageID()
{
    return _pageID;
}

std::string & WebPage::get_pageTitie()
{
    return _pageTitle;
}

std::string & WebPage::get_pageURL()
{
    return _pageURL;
}

std::string & WebPage::get_pageFullText()
{
    return _pageFullText;
}


