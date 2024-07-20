#pragma once
#include "Exception.h"
#include "Log.h"
#include<algorithm>
#include<list>
#include<cmath>
#include<windows.h>

constexpr auto MAX_SIZE = 256 * 1024; //���������������ڴ�

constexpr auto NFREELIST = 208; // ��������Ͱ������±�

constexpr auto NPAGES = 128; // ҳͰ������±�

constexpr auto PAGE_SHIFT = 13; // ��λһ��ҳ��С����Ĵ���

#ifdef _WIN64
typedef unsigned long long PAGE_ID;
#elif _WIN32
typedef size_t PAGE_ID;
#endif

// ֱ�������ϵͳ����ָ��ҳ���ڴ�2
void* SystemAlloc(size_t kpage);

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

	//CentralCache��PageCacheһ�������Span��ҳ��
	static size_t NumAllocSpan(size_t size);
private:
	static size_t _RoundUp(size_t size, size_t Align);
	static size_t _Index(size_t size, size_t Align);
};

// ������������
class FreeList {
public:
	// ����
	void push(void* obj);

	// ����������
	void pushRange(void* start, void* endl);

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
	PAGE_ID _pageId;		// ����ڴ���ʼҳҳ��
	size_t _n;			// ҳ������ 

	void* _list;		// �кõ�С���ڴ桪��ͨ����������
	size_t _useCount;	// �����ڴ������Ϊ0ʱδ������κοռ���߷����ȥ�Ŀռ䶼������

	Span* _next = nullptr;
	Span* _prev = nullptr;

	size_t _objSize;	// ���������С

	bool isUse = false; // �Ƿ�����ʹ��
};

class SpanList {
public:
	// ��/����
	void lock();
	void unlock();

	// ������
	std::list<Span*>::iterator begin();
	std::list<Span*>::iterator end();

	//�п�
	bool empty();

	//����һ��Span
	Span* PopFront();
	//����һ��Span
	void PushFront(Span* span);
private:
	std::list<Span*> _list;
	std::mutex _mtx;
};