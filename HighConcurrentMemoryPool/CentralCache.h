#pragma once
#include "common.h"

//单例模式
class CentralCache {
public:
	//申请空间
	void* allocMember(int index);
	//释放空间
	void releaseMember(void* front, int size, int len);

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
		GC();
		~GC();
	};

	static GC _GC;
	static CentralCache* _cc;
	static std::mutex _mtx;

	SpanList _span[NFREELIST];
};




