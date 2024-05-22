#include "Exception.h"

Exception::Exception(int errid, std::string errmsg)
    :_errid(errid)
    , _errmsg(errmsg)
{
    ProjectLog->record("[Error" + std::to_string(_errid) + "]:" +  _errmsg);
}

int Exception::geterrid()
{
    return _errid;
}

std::string Exception::geterrmsg()
{
    return _errmsg;
}


