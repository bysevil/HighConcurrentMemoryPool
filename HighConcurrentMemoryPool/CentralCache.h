#pragma once
#include "common.h"
#include "Log.h"
#include "Exception.h"

class CentralCache {
public:
	//����ռ�
	void* allocMember();
	//�ͷſռ�
	void releaseMember();
private:
	CentralCache();
};

