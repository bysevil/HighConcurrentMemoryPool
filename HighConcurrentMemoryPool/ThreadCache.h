#pragma once
#include "common.h"

constexpr auto MAX_SIZE = 256*1024;




//����ģʽ
class ThreadCache {
public:

	//����ռ�
	void* allocMemory(size_t alloc_byte);

	//�ͷſռ�
	void releaseMemory(void* obj, size_t obj_byte);
	
private:
	
	FreeList _freeList[NFREELIST]; //��������
};

//����ThreadCache
ThreadCache* CreateThreadCache();
//����ThreadCache
void DestructionThreadCache();

