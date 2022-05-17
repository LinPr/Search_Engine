#pragma once 

#include <string>
#include <vector>
#include <iostream>


class Config
{

public:
    Config(std::string configFilePath);

    void readConfigFile();
    std::string get_xmlDirectoryPath();
    std::string get_stopWordsPath();
    std::string get_parsedPagesPath();
    
    void showAllPaths();

private:
    std::string _xmlDirectoryPath;
    std::string _stopWordsPath;
    std::string _parsedPagesPath;


private:
    std::string _configFilePath;
};
