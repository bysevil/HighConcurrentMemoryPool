#pragma once
#include "common.h"

class ThreadCache {
public:
	//分配空间给对象
	void* allocMemory(size_t alloc_byte);

	//对象还回空间
	void releaseMemory(void* obj, size_t obj_byte);

	//向中心缓存申请空间
	void FetchFromCentralCache(size_t index, size_t size);
private:
	//使用慢反馈调节算法调节单次申请的内存数量
	size_t bathItemNum(size_t index, size_t size);

	FreeList _freeMap[NFREELIST]; //自由链表
};

#ifdef _WIN32
static __declspec(thread) ThreadCache* TLS_tc = nullptr;
#else
static _thread ThreadCache* TLS_tc = nullptr;
#endif


