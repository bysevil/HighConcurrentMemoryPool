#pragma once
#include "Exception.h"

constexpr auto MAX_ALLOC_SIZE = 256*1024;

class ThreadCache {
public:
	ThreadCache();

	//����ռ�
	void* allocMemory(size_t alloc_byte);

	//�ͷſռ�
	void releaseMemory(void* obj, size_t obj_byte);
	

private:
	class FreeList {

	};

	void* _memoryFront; //���пռ�ͷ
	FreeList* _freeList; //��������
	size_t _residueByte; // ʣ��ռ��С
};


#ifdef _WIN32
	__declspec(thread) static ThreadCache* threadCache = new ThreadCache();
#else
	_thread static ThreadCache* threadCache = new ThreadCache();
#endif
