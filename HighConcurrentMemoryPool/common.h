#pragma once
#include "Exception.h"

constexpr auto NFREELIST = 208;

static void*& nextObj(void* obj);

class BitAlignment {
	// ������������10%���ҵ�����Ƭ�˷�
	// [1,128]					8byte����	    freelist[0,16)
	// [128+1,1024]				16byte����	    freelist[16,72)
	// [1024+1,8*1024]			128byte����	    freelist[72,128)
	// [8*1024+1,64*1024]		1024byte����     freelist[128,184)
	// [64*1024+1,256*1024]		8*1024byte����   freelist[184,208)
public:
	//���������
	static inline size_t RoundUp(size_t size);

	//����λ���ĸ�Ͱ
	static inline size_t Index(size_t size);
private:
	static inline size_t _RoundUp(size_t size, size_t Align);
	static inline size_t _Index(size_t size, size_t Align);
};

class FreeList {
public:
	//����
	void push(void* obj);

	//ɾ��
	void* pop();

	//�п�
	bool empty();
private:
	void* _freeList;
};