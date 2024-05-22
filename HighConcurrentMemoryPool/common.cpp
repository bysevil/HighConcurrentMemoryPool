#include "common.h"


void FreeList::push(void* obj)
{
    nextObj(obj) = _freeList;
    _freeList = obj;
}

void* FreeList::pop()
{
    void* ret = _freeList;
    _freeList = nextObj(_freeList);
    return ret;
}

bool FreeList::empty()
{
	return _freeList == nullptr;
}

void*& nextObj(void* obj)
{
    return *(void**)obj;
}

inline size_t BitAlignment::_RoundUp(size_t size, size_t Align)
{
	return ((size + Align - 1) & ~(Align - 1));
}

inline size_t BitAlignment::RoundUp(size_t size)
{
	if (size <= 128)
	{
		return _RoundUp(size, 8);
	}
	else if (size <= 1024)
	{
		return _RoundUp(size, 16);
	}
	else if (size <= 8 * 1024)
	{
		return _RoundUp(size, 128);
	}
	else if (size <= 64 * 1024)
	{
		return _RoundUp(size, 1024);
	}
	else if (size <= 256 * 1024)
	{
		return _RoundUp(size, 8 * 1024);
	}
	else
	{
		throw Exception(3,"Failed RoundUp - size greater then MAX_ALLOC_SIZE");
	}
}

inline size_t BitAlignment::_Index(size_t size, size_t Align)
{
	return ((size + (1 << Align) - 1) >> Align) - 1;
}

inline size_t BitAlignment::Index(size_t bytes)
{
	static int group_array[4] = { 16, 56, 56, 56 };
	if (bytes <= 128) {
		return _Index(bytes, 3);
	}
	else if (bytes <= 1024) {
		return _Index(bytes - 128, 4) + group_array[0];
	}
	else if (bytes <= 8 * 1024) {
		return _Index(bytes - 1024, 7) + group_array[1] + group_array[0];
	}
	else if (bytes <= 64 * 1024) {
		return _Index(bytes - 8 * 1024, 10) + group_array[2] + group_array[1] + group_array[0];
	}
	else if (bytes <= 256 * 1024) {
		return _Index(bytes - 64 * 1024, 13) + group_array[3] + group_array[2] + group_array[1] + group_array[0];
	}
	else {
		throw Exception(4, "Failed Index - Bucket does not exist");
	}
}




