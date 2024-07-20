#include "common.h"

void* SystemAlloc(size_t kpage)
{
#ifdef _WIN32
	void* ptr = VirtualAlloc(0, kpage << 13, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	// linux下brk mmap等
#endif

	if (ptr == nullptr)
		throw std::bad_alloc();
	Log::record(Log::INFO, "向操作系统申请内存成功,大小：" + std::to_string(kpage << 13) + "起始地址：" + std::to_string((int)ptr));
	return ptr;
}

void FreeList::push(void* obj)
{
	Log::record(Log::INFO, "将对象插入自由链表");
	
	//头插
	nextObj(obj) = _freeList;
	_freeList = obj;

	Log::record(Log::INFO,"将对象插入自由链表成功");
}

void FreeList::pushRange(void* start, void* endl)
{
	Log::record(Log::INFO, "将对象序列插入自由链表");
	nextObj(endl) = _freeList;
	_freeList = start;
	Log::record(Log::INFO, "将对象序列插入自由链表成功");
}

void* FreeList::pop() 
{
	Log::record(Log::INFO, "从自由链表中弹出一个对象");

	//保证_freeList不为空
	if (empty()) {
		throw Exception(1,"自由链表为空，弹出对象失败");
	}
	//头删
	void* ret = _freeList;
	_freeList = nextObj(_freeList);
	
	Log::record(Log::INFO,"成功从自由链表中弹出一个对象");

    return ret;
}

bool FreeList::empty()
{
	bool ret = _freeList == nullptr;
	Log::record(Log::INFO, "判断自由链表是否为空：" + std::to_string(ret));
	return ret;
}

size_t FreeList::getMaxSize()
{
	Log::record(Log::INFO, "获取自由链表的maxSize：" + std::to_string(_maxSize));
	return _maxSize;
}

void FreeList::setMaxSize()
{
	_maxSize += 1;
	Log::record(Log::INFO, "修改自由链表的maxSize，修改后为：" + std::to_string(_maxSize));
}

void*& nextObj(void* obj)
{
    return *(void**)obj;
}

size_t BitAlignment::MaxAllocItem(size_t size)
{
	Log::record(Log::INFO, "计算向CentralCache申请的内存数量");
	if (size <= 0) {
		throw Exception(1, "计算向CentralCache申请的内存数量失败：size <= 0");
	}

	// [2, 512]，一次批量移动多少个对象的(慢启动)上限值
	// 小对象一次批量上限高
	// 小对象一次批量上限低
	int num = MAX_SIZE / size;
	if (num < 2)
		num = 2;

	if (num > 512)
		num = 512;

	Log::record(Log::INFO,"计算向CentralCache申请的内存数量成功，返回：" + std::to_string(num));
	return num;
}

size_t BitAlignment::NumAllocSpan(size_t size)
{
	Log::record(Log::INFO, "开始计算需要申请的Span的页数");

	size_t num = MaxAllocItem(size);
	size_t npage = num * size;
	npage >>= PAGE_SHIFT;
	if (npage < 1) {
		npage = 1;
	}

	Log::record(Log::INFO, "计算需要申请的Span的页数成功：返回"+std::to_string(npage));
	return npage;
}

size_t BitAlignment::_RoundUp(size_t size, size_t Align)
{
	return ((size + Align - 1) & ~(Align - 1));
	//向上加一个对齐数到下一个档位，然后再将低位清零让其对齐
	//减一是为了防止本身已经对齐的情况下进入下一个档位导致对齐错误
}

size_t BitAlignment::RoundUp(size_t size)
{
	Log::record(Log::INFO,"计算内存对齐后的大小");

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
		throw Exception(3,"计算内存对齐后的大小失败，size过大");
	}

	Log::record(Log::INFO,"计算内存对齐后的大小成功，返回：" + std::to_string(ret));

	return ret;
}

size_t BitAlignment::_Index(size_t size, size_t Align)
{
	return ((size + (static_cast<unsigned long long>(1) << Align) - 1) >> Align) - 1;
}

size_t BitAlignment::Index(size_t bytes)
{
	Log::record(Log::INFO,"计算指定大小的内存[" + std::to_string(bytes) + "]在自由链表桶的下标");
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
		throw Exception(4, "计算失败，bytes过大");
	}
	Log::record(Log::INFO, "计算指定大小的内存[" + std::to_string(bytes) + "]在自由链表桶的下标成功，返回" + std::to_string(ret));
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
