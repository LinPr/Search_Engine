#include "Config.h"
#include <fstream>
#include <sstream>


Config::Config(std::string configFilePath)
: _configFilePath(configFilePath)
{
    readConfigFile();
}

void Config::readConfigFile()
{

    std::ifstream ifs(_configFilePath);
    std::string line;
    while(getline(ifs, line))
    {   
        std::istringstream line_copy(line);
        std::string pathType, path;
        line_copy >> pathType >> path;
        if(pathType == "xmlDirectoryPaths")     { _xmlDirectoryPath = path; }
        else if(pathType == "stopWordsPath")    { _stopWordsPath    = path; }
        else if(pathType == "parsedPagesPath")  { _parsedPagesPath  = path; }
    }
}


std::string Config::get_xmlDirectoryPath()
{
    if(_xmlDirectoryPath.empty()) 
    { 
        std::cout << "_xmlDirectoryPath empty" << std::endl; 
    } 
    else 
    { 
        return _xmlDirectoryPath; 
    }
}   

std::string Config::get_stopWordsPath()
{
    if(_stopWordsPath.empty()) 
    { 
        std::cout <<  "_stopWordsPath empty" << std::endl; 
    }
    else 
    { 
        return _stopWordsPath; 
    }
}

std::string Config::get_parsedPagesPath()
{
    if(_parsedPagesPath.empty()) 
    { 
        std::cout <<  " _parsedPagesPath.empty" << std::endl; 
    }
    else 
    { 
        return _parsedPagesPath; 
    }
}


void Config::showAllPaths()
{
    std::cout << "_xmlDirectoryPath" << get_xmlDirectoryPath() << std::endl;
    std::cout << "_stopWordsPath" << get_stopWordsPath() << std::endl;
    std::cout << "_parsedPagesPath" << get_parsedPagesPath() << std::endl;
}