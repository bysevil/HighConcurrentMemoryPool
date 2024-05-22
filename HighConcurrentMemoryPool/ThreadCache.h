#pragma once
#include "Exception.h"

constexpr auto MAX_ALLOC_SIZE = 256*1024;

class ThreadCache {
public:
	ThreadCache();

	//申请空间
	void* allocMemory(size_t alloc_byte);

	//释放空间
	void releaseMemory(void* obj, size_t obj_byte);
	

private:
	class FreeList {

	};

	void* _memoryFront; //持有空间头
	FreeList* _freeList; //自由链表
	size_t _residueByte; // 剩余空间大小
};


#ifdef _WIN32
	__declspec(thread) static ThreadCache* threadCache = new ThreadCache();
#else
	_thread static ThreadCache* threadCache = new ThreadCache();
#endif
