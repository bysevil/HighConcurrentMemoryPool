#pragma once
#include"Log.h"

class Exception {
public:
    //ͨ��������ʹ������������쳣,����������ʹ�������������־
    Exception(int errid, std::string errmsg);

    //��ȡ������
    int geterrid();

    //��ȡ��������
    std::string geterrmsg();        
   
private:
    int _errid; //������
    std::string _errmsg; //��������
};