#pragma once
#include "common.h"

constexpr auto MAX_SIZE = 256*1024;




//单例模式
class ThreadCache {
public:

	//申请空间
	void* allocMemory(size_t alloc_byte);

	//释放空间
	void releaseMemory(void* obj, size_t obj_byte);
	
private:
	
	FreeList _freeList[NFREELIST]; //自由链表
};

//创建ThreadCache
ThreadCache* CreateThreadCache();
//销毁ThreadCache
void DestructionThreadCache();

