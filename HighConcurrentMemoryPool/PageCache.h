#pragma once
#include "common.h"

class PageCache {
public:
	// ����/��ȡ�������Ļ���
	static PageCache* CreatePageCache();
	// ���ٵ������Ļ���
	static void DestructionPageCache();

	// ��CentralCache����ָ��ҳ����Span
	Span* allocSpan(size_t size);

private:
	//��Span�з֣����������Ĳ��ֹ���spanMap
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