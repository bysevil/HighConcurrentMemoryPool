#include "common.h"

void* SystemAlloc(size_t kpage)
{
#ifdef _WIN32
	void* ptr = VirtualAlloc(0, kpage << 13, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	// linux��brk mmap��
#endif

	if (ptr == nullptr)
		throw std::bad_alloc();
	Log::record(Log::INFO, "�����ϵͳ�����ڴ�ɹ�,��С��" + std::to_string(kpage << 13) + "��ʼ��ַ��" + std::to_string((int)ptr));
	return ptr;
}

void FreeList::push(void* obj)
{
	Log::record(Log::INFO, "�����������������");
	
	//ͷ��
	nextObj(obj) = _freeList;
	_freeList = obj;

	Log::record(Log::INFO,"�����������������ɹ�");
}

void FreeList::pushRange(void* start, void* endl)
{
	Log::record(Log::INFO, "���������в�����������");
	nextObj(endl) = _freeList;
	_freeList = start;
	Log::record(Log::INFO, "���������в�����������ɹ�");
}

void* FreeList::pop() 
{
	Log::record(Log::INFO, "�����������е���һ������");

	//��֤_freeList��Ϊ��
	if (empty()) {
		throw Exception(1,"��������Ϊ�գ���������ʧ��");
	}
	//ͷɾ
	void* ret = _freeList;
	_freeList = nextObj(_freeList);
	
	Log::record(Log::INFO,"�ɹ������������е���һ������");

    return ret;
}

bool FreeList::empty()
{
	bool ret = _freeList == nullptr;
	Log::record(Log::INFO, "�ж����������Ƿ�Ϊ�գ�" + std::to_string(ret));
	return ret;
}

size_t FreeList::getMaxSize()
{
	Log::record(Log::INFO, "��ȡ���������maxSize��" + std::to_string(_maxSize));
	return _maxSize;
}

void FreeList::setMaxSize()
{
	_maxSize += 1;
	Log::record(Log::INFO, "�޸����������maxSize���޸ĺ�Ϊ��" + std::to_string(_maxSize));
}

void*& nextObj(void* obj)
{
    return *(void**)obj;
}

size_t BitAlignment::MaxAllocItem(size_t size)
{
	Log::record(Log::INFO, "������CentralCache������ڴ�����");
	if (size <= 0) {
		throw Exception(1, "������CentralCache������ڴ�����ʧ�ܣ�size <= 0");
	}

	// [2, 512]��һ�������ƶ����ٸ������(������)����ֵ
	// С����һ���������޸�
	// С����һ���������޵�
	int num = MAX_SIZE / size;
	if (num < 2)
		num = 2;

	if (num > 512)
		num = 512;

	Log::record(Log::INFO,"������CentralCache������ڴ������ɹ������أ�" + std::to_string(num));
	return num;
}

size_t BitAlignment::NumAllocSpan(size_t size)
{
	Log::record(Log::INFO, "��ʼ������Ҫ�����Span��ҳ��");

	size_t num = MaxAllocItem(size);
	size_t npage = num * size;
	npage >>= PAGE_SHIFT;
	if (npage < 1) {
		npage = 1;
	}

	Log::record(Log::INFO, "������Ҫ�����Span��ҳ���ɹ�������"+std::to_string(npage));
	return npage;
}

size_t BitAlignment::_RoundUp(size_t size, size_t Align)
{
	return ((size + Align - 1) & ~(Align - 1));
	//���ϼ�һ������������һ����λ��Ȼ���ٽ���λ�����������
	//��һ��Ϊ�˷�ֹ�����Ѿ����������½�����һ����λ���¶������
}

size_t BitAlignment::RoundUp(size_t size)
{
	Log::record(Log::INFO,"�����ڴ�����Ĵ�С");

	size_t ret = 0;
	if (size <= 128)
	{
		ret = _RoundUp(size, 8);
	}
	else if (size <= 1024)
	{
		ret = _RoundUp(size, 16);
	}
	else if (size <= 8 * 1024)
	{
		ret = _RoundUp(size, 128);
	}
	else if (size <= 64 * 1024)
	{
		ret = _RoundUp(size, 1024);
	}
	else if (size <= 256 * 1024)
	{
		ret = _RoundUp(size, 8 * 1024);
	}
	else
	{
		throw Exception(3,"�����ڴ�����Ĵ�Сʧ�ܣ�size����");
	}

	Log::record(Log::INFO,"�����ڴ�����Ĵ�С�ɹ������أ�" + std::to_string(ret));

	return ret;
}

size_t BitAlignment::_Index(size_t size, size_t Align)
{
	return ((size + (static_cast<unsigned long long>(1) << Align) - 1) >> Align) - 1;
}

size_t BitAlignment::Index(size_t bytes)
{
	Log::record(Log::INFO,"����ָ����С���ڴ�[" + std::to_string(bytes) + "]����������Ͱ���±�");
	static int group_array[4] = { 16, 56, 56, 56 };
	size_t ret = 0;
	if (bytes <= 128) {
		ret = _Index(bytes, 3);
	}
	else if (bytes <= 1024) {
		ret = _Index(bytes - 128, 4) + group_array[0];
	}
	else if (bytes <= 8 * 1024) {
		ret = _Index(bytes - 1024, 7) + group_array[1] + group_array[0];
	}
	else if (bytes <= 64 * 1024) {
		ret = _Index(bytes - 8 * 1024, 10) + group_array[2] + group_array[1] + group_array[0];
	}
	else if (bytes <= 256 * 1024) {
		ret = _Index(bytes - 64 * 1024, 13) + group_array[3] + group_array[2] + group_array[1] + group_array[0];
	}
	else {
		throw Exception(4, "����ʧ�ܣ�bytes����");
	}
	Log::record(Log::INFO, "����ָ����С���ڴ�[" + std::to_string(bytes) + "]����������Ͱ���±�ɹ�������" + std::to_string(ret));
	return ret;
}

void SpanList::lock()
{
	_mtx.lock();
}

void SpanList::unlock()
{
	_mtx.unlock();
}

std::list<Span*>::iterator SpanList::begin()
{
	return _list.begin();
}

std::list<Span*>::iterator SpanList::end()
{
	return _list.end();
}

bool SpanList::empty()
{
	return _list.empty();
}

Span* SpanList::PopFront()
{
	Span* ret = _list.front();
	_list.pop_front();
	return ret;
}

void SpanList::PushFront(Span* span)
{
	lock();
	_list.push_front(span);
	unlock();
}
