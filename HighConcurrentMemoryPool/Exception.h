#pragma once
#include <string>

class Exception {
public:
    //ͨ��������ʹ������������쳣,����������ʹ�������������־
    Exception(int errid, std::string errmsg)
        :_errid(errid)
        , _errmsg(errmsg)
    {}

    //��ȡ������
    int geterrid() {
        return _errid;
    }

    //��ȡ��������
    std::string geterrmsg() {
        return _errmsg;
    }
   
private:
    int _errid; //������
    std::string _errmsg; //��������
};

