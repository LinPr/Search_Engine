#include "ProtocolUnit.h" 
#include <sstream> 


 
/* 用于数据拆包 */
ProtocolUnit::ProtocolUnit(std::string & msg)
{
    std::istringstream iss_msg(msg);
    iss_msg >> _protocalID ;
    getline(iss_msg, _rawData);
}

/* 用于数据封包 */
ProtocolUnit::ProtocolUnit(int protocal_ID, std::string & rawData )
{
    std::ostringstream oss_msg;
    oss_msg << protocal_ID << " " << rawData;
    _msg = oss_msg.str();
}

int & ProtocolUnit::get_protocalID()
{
    return _protocalID;
}

std::string & ProtocolUnit::get_rawData()
{
    return _rawData;
}

std::string & ProtocolUnit::get_msg()
{
    return _msg;
}