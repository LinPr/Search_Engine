#pragma once 

class NonCopyable
{
private:
    /* data */
public:
    NonCopyable() {} ;
    ~NonCopyable() {} ;

    NonCopyable(const NonCopyable & ) = delete;
    NonCopyable & operator=(const NonCopyable &) = delete;
};


