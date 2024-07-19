#include "common.h"

//线程向中心缓存申请资源的次数，用来使用慢反馈调节算法调节中心缓存一次性给予的空间数目
//#ifdef _WIN32
//__declspec(thread) size_t CentralCacheAllocNum = 0;
//#else
//_thread size_t CentralCacheAllocNum = 0;
//#endif

void FreeList::push(void* obj)
{
	Log::record(Log::INFO, "freeList Push start");
	
	//头插
	nextObj(obj) = _freeList;
	_freeList = obj;

	Log::record(Log::INFO,"freeList Push endl");
}

void* FreeList::pop() 
{
	Log::record(Log::INFO, "freeList Pop start");

	//保证_freeList不为空
	if (empty()) {
		throw Exception(1,"pop nullptr freeList");
	}
	//头删
	void* ret = _freeList;
	_freeList = nextObj(_freeList);
	
	Log::record(Log::INFO,"freeList Pop endl");

    return ret;
}

bool FreeList::empty()
{
	return _freeList == nullptr;
}

size_t FreeList::getMaxSize()
{
	return _maxSize;
}

void FreeList::setMaxSize()
{
	_maxSize += 1;
}

void*& nextObj(void* obj)
{
    return *(void**)obj;
}

size_t BitAlignment::MaxAllocItem(size_t size)
{
	if (size <= 0) {
		throw Exception(1, "Failed NumMoveSize : size less then zero");
	}

	// [2, 512]，一次批量移动多少个对象的(慢启动)上限值
	// 小对象一次批量上限高
	// 小对象一次批量上限低
	int num = MAX_SIZE / size;
	if (num < 2)
		num = 2;

	if (num > 512)
		num = 512;

	return num;
}

size_t BitAlignment::_RoundUp(size_t size, size_t Align)
{
	return ((size + Align - 1) & ~(Align - 1));
	//向上加一个对齐数到下一个档位，然后再将低位清零让其对齐
	//减一是为了防止本身已经对齐的情况下进入下一个档位导致对齐错误
}

size_t BitAlignment::RoundUp(size_t size)
{
	Log::record(Log::INFO,"execute bitAligment::RoudUp");

	size_t ret = 0;
	if (size <= 128)
	{
		ret = _RoundUp(size, 8);
	}
	else if (size <= 1024)
	{
		ret = _RoundUp(size, 16);
	}
	else if (size <= 8 * 1024)
	{
		ret = _RoundUp(size, 128);
	}
	else if (size <= 64 * 1024)
	{
		ret = _RoundUp(size, 1024);
	}
	else if (size <= 256 * 1024)
	{
		ret = _RoundUp(size, 8 * 1024);
	}
	else
	{
		throw Exception(3,"Failed RoundUp - size greater then MAX_ALLOC_SIZE");
	}

	Log::record(Log::INFO,"bitAligment::RoudUp return" + std::to_string(ret));

	return ret;
}

size_t BitAlignment::_Index(size_t size, size_t Align)
{
	return ((size + (static_cast<unsigned long long>(1) << Align) - 1) >> Align) - 1;
}

size_t BitAlignment::Index(size_t bytes)
{
	Log::record(Log::INFO,"execute bitAligment::Index");
	static int group_array[4] = { 16, 56, 56, 56 };
	size_t ret = 0;
	if (bytes <= 128) {
		ret = _Index(bytes, 3);
	}
	else if (bytes <= 1024) {
		ret = _Index(bytes - 128, 4) + group_array[0];
	}
	else if (bytes <= 8 * 1024) {
		ret = _Index(bytes - 1024, 7) + group_array[1] + group_array[0];
	}
	else if (bytes <= 64 * 1024) {
		ret = _Index(bytes - 8 * 1024, 10) + group_array[2] + group_array[1] + group_array[0];
	}
	else if (bytes <= 256 * 1024) {
		ret = _Index(bytes - 64 * 1024, 13) + group_array[3] + group_array[2] + group_array[1] + group_array[0];
	}
	else {
		throw Exception(4, "Failed Index - Bucket does not exist");
	}
	Log::record(Log::INFO,"bitAligment::Index return" + std::to_string(ret));
	return ret;
}

