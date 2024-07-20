#pragma once
#include "common.h"

//单例模式
class CentralCache {
public:
	// 获取一个非空的Span
	Span* getOneSpan(size_t index, size_t size);

	// 向threadCache分配空间
	size_t allocRangeMember(void*& start, void*& endl, size_t batchNum, size_t size, size_t index);
	
	//释放空间
	//void releaseMember(void* front, int size, int len);

	//创建单例中心缓存
	static CentralCache* CreateCentralCache();
	//销毁单例中心缓存
	static void DestructionCentralCache();
private:
	CentralCache();
	~CentralCache();
	CentralCache(const CentralCache&) = delete;
	CentralCache operator=(const CentralCache&) = delete;
	struct GC {
		~GC();
	};

	static GC _GC;
	static CentralCache* _cc;
	static std::mutex _mtx;

	SpanList _spanMap[NFREELIST];
};




