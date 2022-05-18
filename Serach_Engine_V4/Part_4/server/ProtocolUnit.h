#pragma once
#include <string>


class ProtocolUnit
{
public:
    /* 用于数据拆包 */
    ProtocolUnit(std::string & msg);
    /* 用于数据封包 */
    ProtocolUnit(int protocal_ID, std::string & rawData);


    int & get_protocalID();
    std::string & get_rawData();
    std::string & get_msg();


private:
    int _protocalID;
    std::string _rawData;
    std::string _msg;
};
