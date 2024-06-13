#include "Exception.h"

Exception::Exception(int errid, std::string errmsg)
    :_errid(errid)
    , _errmsg(errmsg)
{}

int Exception::geterrid()
{
    return _errid;
}

std::string Exception::geterrmsg()
{
    return _errmsg;
}


