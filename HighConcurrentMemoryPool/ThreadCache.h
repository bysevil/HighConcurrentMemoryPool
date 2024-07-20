#pragma once
#include "common.h"

class ThreadCache {
public:
	//����ռ������
	void* allocMemory(size_t alloc_byte);

	//���󻹻ؿռ�
	void releaseMemory(void* obj, size_t obj_byte);

	//�����Ļ�������ռ�
	void FetchFromCentralCache(size_t index, size_t size);
private:
	//ʹ�������������㷨���ڵ���������ڴ�����
	size_t bathItemNum(size_t index, size_t size);

	FreeList _freeMap[NFREELIST]; //��������
};

#ifdef _WIN32
static __declspec(thread) ThreadCache* TLS_tc = nullptr;
#else
static _thread ThreadCache* TLS_tc = nullptr;
#endif


