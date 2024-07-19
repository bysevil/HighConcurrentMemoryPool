#pragma once
#include "common.h"

//����ģʽ
class CentralCache {
public:
	//����ռ�
	void* allocMember(int index);
	//�ͷſռ�
	void releaseMember(void* front, int size, int len);

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
		GC();
		~GC();
	};

	static GC _GC;
	static CentralCache* _cc;
	static std::mutex _mtx;

	SpanList _span[NFREELIST];
};




