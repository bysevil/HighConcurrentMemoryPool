#pragma once
#include "Exception.h"

constexpr auto NFREELIST = 208;

static void*& nextObj(void* obj);

class BitAlignment {
	// 整体控制在最多10%左右的内碎片浪费
	// [1,128]					8byte对齐	    freelist[0,16)
	// [128+1,1024]				16byte对齐	    freelist[16,72)
	// [1024+1,8*1024]			128byte对齐	    freelist[72,128)
	// [8*1024+1,64*1024]		1024byte对齐     freelist[128,184)
	// [64*1024+1,256*1024]		8*1024byte对齐   freelist[184,208)
public:
	//计算对齐数
	static inline size_t RoundUp(size_t size);

	//计算位于哪个桶
	static inline size_t Index(size_t size);
private:
	static inline size_t _RoundUp(size_t size, size_t Align);
	static inline size_t _Index(size_t size, size_t Align);
};

class FreeList {
public:
	//插入
	void push(void* obj);

	//删除
	void* pop();

	//判空
	bool empty();
private:
	void* _freeList;
};