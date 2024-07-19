#pragma once
#include <string>

class Exception {
public:
    //通过错误码和错误描述创建异常,并将错误码和错误描述记入日志
    Exception(int errid, std::string errmsg)
        :_errid(errid)
        , _errmsg(errmsg)
    {}

    //获取错误码
    int geterrid() {
        return _errid;
    }

    //获取错误描述
    std::string geterrmsg() {
        return _errmsg;
    }
   
private:
    int _errid; //错误码
    std::string _errmsg; //错误描述
};

