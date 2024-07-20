#pragma once
#include "common.h"

//����ģʽ
class CentralCache {
public:
	// ��ȡһ���ǿյ�Span
	Span* getOneSpan(size_t index, size_t size);

	// ��threadCache����ռ�
	size_t allocRangeMember(void*& start, void*& endl, size_t batchNum, size_t size, size_t index);
	
	//�ͷſռ�
	//void releaseMember(void* front, int size, int len);

	//�����������Ļ���
	static CentralCache* CreateCentralCache();
	//���ٵ������Ļ���
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




