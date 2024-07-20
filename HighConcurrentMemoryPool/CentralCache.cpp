#include "CentralCache.h"
#include "PageCache.h"

std::mutex CentralCache::_mtx;
CentralCache* CentralCache::_cc = CentralCache::CreateCentralCache();
CentralCache::GC CentralCache::_GC;

CentralCache::GC::~GC() {
    DestructionCentralCache();
}

Span* CentralCache::getOneSpan(size_t index, size_t size)
{
    Log::record(Log::INFO, "开始获取一个非空的跨度");

    _spanMap[index].lock();
    for (auto e : _spanMap[index]) {
        if (e->_list != nullptr) {
            _spanMap[index].unlock();
            return e;
        }
    }
    _spanMap[index].unlock();

    Span* span = PageCache::CreatePageCache()->allocSpan(BitAlignment::NumAllocSpan(size));
    if (span == nullptr) {
        throw Exception(1, "向PageCache申请Span失败，其返回nullptr");
    }
    // 获取申请到的Span的大块内存的起始和结束位置
    char* address = (char*)(span->_pageId << PAGE_SHIFT);
    if (address == nullptr) {
        throw Exception(1, "向PageCache申请Span失败，Span管理nullptr");
    }
    size_t bytes = span->_n << PAGE_SHIFT; 
    char* endl = address + bytes; 

    // 将大块内存切成自由链表
    span->_list = address;
    char* cur = address;
    while (cur < endl) {
        nextObj(cur) = cur + size;
        cur += size;
    }
    nextObj(cur - size) = nullptr;

    _spanMap[index].PushFront(span);

    Log::record(Log::INFO, "获取一个非空跨度成功");
    return span;
}

size_t CentralCache::allocRangeMember(void*& start, void*& endl, size_t batchNum, size_t size, size_t index)
{
    Log::record(Log::INFO,"开始向线程缓存分配内存");

    Span* span = getOneSpan(index, size);

    

    if (span == nullptr) {
        throw Exception(1,"向线程缓存分配内存失败，未获取到跨度");
    }
    if (span->_list == nullptr) {
        throw Exception(1, "向线程缓存分配内存失败，获取到空的跨度");
    }

    Log::record(Log::INFO, "开始构建小块内存序列");

    _spanMap[index].lock();
    start = span->_list;
    endl = start;
    int num = 1;
    while(num < batchNum && nextObj(endl) != nullptr) {
        endl = nextObj(endl);
        num++;
    }
    span->_list = nextObj(endl);
    nextObj(endl) = nullptr;   
    span->_useCount += num;

    _spanMap[index].unlock();
    
    Log::record(Log::INFO, "构建小块内存序列成功");

    Log::record(Log::INFO, "向线程缓存分配内存成功");

    return num;
}

CentralCache* CentralCache::CreateCentralCache()
{
    if (_cc == nullptr) {
        _mtx.lock();
        if (_cc == nullptr) {
            _cc = new CentralCache();
        }
        _mtx.unlock();
    }
    return _cc;
}

void CentralCache::DestructionCentralCache()
{
    if (_cc != nullptr) {
        _mtx.lock();
        if (_cc != nullptr) {
            delete _cc;
        }
        _mtx.unlock();
        _cc = nullptr;
    }
}

CentralCache::~CentralCache()
{
}

CentralCache::CentralCache(){
}