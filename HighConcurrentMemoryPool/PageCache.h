#pragma once
#include "common.h"

class PageCache {
public:
	// 创建/获取单例中心缓存
	static PageCache* CreatePageCache();
	// 销毁单例中心缓存
	static void DestructionPageCache();

	// 向CentralCache分配指定页数的Span
	Span* allocSpan(size_t size);

private:
	//将Span切分，将切下来的部分挂入spanMap
	void sectionSpan(Span* span, size_t n, size_t newn);

	SpanList _spanMap[NPAGES];
	std::mutex _mtx;

	PageCache();
	~PageCache();
	PageCache(const PageCache&) = delete;
	PageCache& operator=(const PageCache&) = delete;

	struct GC {
		~GC();
	};

	static PageCache* _pc;
	static GC _gc;
	static std::mutex _pcMtx;
};