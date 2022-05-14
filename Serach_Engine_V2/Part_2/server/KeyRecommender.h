#pragma once

#include "Dictionary.h"
// #include "TcpConnection.h"
#include <iostream>
#include <string>
#include <vector>

class KeyRecommender
{
public:
    KeyRecommender(Dictionary & _dictionary);

public:
    bool fuzzyMatching(std::string _userInputWord); // std::string userInputWord
    std::vector<std::string> & getRecommendWords();

private:
    int minEditDistence(std::string word1, std::string word2);

private:
    
    // TcpConnectionPtr & pconnection_manager;
    std::vector<std::string> _recommendWords;
    Dictionary & _dictionary;

};


