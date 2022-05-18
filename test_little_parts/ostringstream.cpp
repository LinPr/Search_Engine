#include <iostream>
#include <sstream>
#include <fstream>

int main()
{
    /* 将上述信息综合生成 page */
    // std::ostringstream ossPage;
    std::string str;
    std::ofstream ofsPage("file.xml");
    std::ostringstream ossPage;
    ossPage << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    std::cout << str << std::endl;
    ossPage << "<doc>";
    ossPage << "<docid>" << 123 << "</docid>";
    ossPage << "<url>" << "_pageURL" << "</url>";
    ossPage << "</doc>";
    // std::cout << ossPage << std::endl;

    str = ossPage.str();
    std::cout << str << std::endl;


}