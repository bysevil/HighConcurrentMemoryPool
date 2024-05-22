#pragma once
#include "Exception.h"
#include "common.h"

constexpr auto MAX_ALLOC_SIZE = 256*1024;

class ThreadCache {
public:

	//申请空间
	void* allocMemory(size_t alloc_byte);

	//释放空间
	void releaseMemory(void* obj, size_t obj_byte);
	

private:
	
	FreeList _freeList[NFREELIST]; //自由链表
};


#ifdef _WIN32
	__declspec(thread) static ThreadCache* TLSThreadCache = nullptr;
#else
	_thread static ThreadCache* TLSThreadCache = nullptr;
#endif
