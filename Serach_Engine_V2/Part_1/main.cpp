#include "DictionaryProducer.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

int main()
{
    string directoryPath = "./CorpusLib/English";
    string stopWordPath = "./StopWordsLib/stop_words_eng.txt";
    string storePath = "./StoreDictionary"; //存储字典其索引的绝对路径

    DictionaryProducer dictionary(directoryPath, stopWordPath, storePath);

    dictionary.showFilePathes();
    dictionary.buildEnDictionary();
    // dictionary.showDictionary();
    dictionary.buildEnDictionaryIndex();
    dictionary.storeDictionary(storePath);
}