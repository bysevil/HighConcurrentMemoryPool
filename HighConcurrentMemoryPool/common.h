#pragma once
#include "Exception.h"
#include "Log.h"
#include<algorithm>
#include<list>
#include<cmath>

constexpr auto NFREELIST = 208; // 桶的最大下标

#ifdef _WIN64
typedef unsigned long long PAGE_ID;
#elif _WIN32
typedef size_t PAGE_ID;
#endif

//获取链表的下个节点
void*& nextObj(void* obj);

// 进行内存对齐和桶下标计算
class BitAlignment {
	// 整体控制在最多10%左右的内碎片浪费
	// [1,128]					8byte对齐	    freelist[0,16)
	// [128+1,1024]				16byte对齐	    freelist[16,72)
	// [1024+1,8*1024]			128byte对齐	    freelist[72,128)
	// [8*1024+1,64*1024]		1024byte对齐    freelist[128,184)
	// [64*1024+1,256*1024]		8*1024byte对齐  freelist[184,208)
public:
	// 计算对齐后的大小
	static size_t RoundUp(size_t size);

	// 通过对齐后的大小计算位于哪个桶
	static size_t Index(size_t size);

	//threadCache从CentralCahce单次申请的内存的数量上限
	static size_t MaxAllocItem(size_t size);
private:
	static size_t _RoundUp(size_t size, size_t Align);
	static size_t _Index(size_t size, size_t Align);
};

// 管理自由链表
class FreeList {
public:
	// 插入
	void push(void* obj);

	// 删除
	void* pop();

	// 判空
	bool empty();

	// 获取maxSize
	size_t getMaxSize();

	// 修改maxSize
	void setMaxSize();
private:
	size_t _maxSize = 1; //调节向上层申请的内存数量
	void* _freeList = nullptr;
};

//管理多个跨度的内存
struct Span {
	int _pageId;		// 大块内存起始页页号
	size_t n;			// 页的数量 

	void* _list;		// 切好的小块内存——通过链表连接
	size_t _useCount;	// 分配内存计数，为0时未分配出任何空间或者分配出去的空间都回来了

	Span* _next = nullptr;
	Span* _prev = nullptr;

	size_t _objSize;	// 单个对象大小

	bool isUse = false; // 是否正在使用
};

// 带头双向循环链表
class SpanList {

private:
	std::list<Span> _list;
};