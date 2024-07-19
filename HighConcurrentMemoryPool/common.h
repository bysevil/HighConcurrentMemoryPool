#pragma once
#include "Exception.h"
#include "Log.h"
#include<algorithm>
#include<list>
#include<cmath>

constexpr auto NFREELIST = 208; // Ͱ������±�

#ifdef _WIN64
typedef unsigned long long PAGE_ID;
#elif _WIN32
typedef size_t PAGE_ID;
#endif

//��ȡ������¸��ڵ�
void*& nextObj(void* obj);

// �����ڴ�����Ͱ�±����
class BitAlignment {
	// ������������10%���ҵ�����Ƭ�˷�
	// [1,128]					8byte����	    freelist[0,16)
	// [128+1,1024]				16byte����	    freelist[16,72)
	// [1024+1,8*1024]			128byte����	    freelist[72,128)
	// [8*1024+1,64*1024]		1024byte����    freelist[128,184)
	// [64*1024+1,256*1024]		8*1024byte����  freelist[184,208)
public:
	// ��������Ĵ�С
	static size_t RoundUp(size_t size);

	// ͨ�������Ĵ�С����λ���ĸ�Ͱ
	static size_t Index(size_t size);

	//threadCache��CentralCahce����������ڴ����������
	static size_t MaxAllocItem(size_t size);
private:
	static size_t _RoundUp(size_t size, size_t Align);
	static size_t _Index(size_t size, size_t Align);
};

// ������������
class FreeList {
public:
	// ����
	void push(void* obj);

	// ɾ��
	void* pop();

	// �п�
	bool empty();

	// ��ȡmaxSize
	size_t getMaxSize();

	// �޸�maxSize
	void setMaxSize();
private:
	size_t _maxSize = 1; //�������ϲ�������ڴ�����
	void* _freeList = nullptr;
};

//��������ȵ��ڴ�
struct Span {
	int _pageId;		// ����ڴ���ʼҳҳ��
	size_t n;			// ҳ������ 

	void* _list;		// �кõ�С���ڴ桪��ͨ����������
	size_t _useCount;	// �����ڴ������Ϊ0ʱδ������κοռ���߷����ȥ�Ŀռ䶼������

	Span* _next = nullptr;
	Span* _prev = nullptr;

	size_t _objSize;	// ���������С

	bool isUse = false; // �Ƿ�����ʹ��
};

// ��ͷ˫��ѭ������
class SpanList {

private:
	std::list<Span> _list;
};