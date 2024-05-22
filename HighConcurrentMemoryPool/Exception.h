#pragma once
#include"Log.h"

class Exception {
public:
    //通过错误码和错误描述创建异常,并将错误码和错误描述记入日志
    Exception(int errid, std::string errmsg);

    //获取错误码
    int geterrid();

    //获取错误描述
    std::string geterrmsg();        
   
private:
    int _errid; //错误码
    std::string _errmsg; //错误描述
};