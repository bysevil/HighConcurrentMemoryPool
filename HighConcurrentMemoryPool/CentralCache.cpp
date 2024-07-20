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
    Log::record(Log::INFO, "��ʼ��ȡһ���ǿյĿ��");

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
        throw Exception(1, "��PageCache����Spanʧ�ܣ��䷵��nullptr");
    }
    // ��ȡ���뵽��Span�Ĵ���ڴ����ʼ�ͽ���λ��
    char* address = (char*)(span->_pageId << PAGE_SHIFT);
    if (address == nullptr) {
        throw Exception(1, "��PageCache����Spanʧ�ܣ�Span����nullptr");
    }
    size_t bytes = span->_n << PAGE_SHIFT; 
    char* endl = address + bytes; 

    // ������ڴ��г���������
    span->_list = address;
    char* cur = address;
    while (cur < endl) {
        nextObj(cur) = cur + size;
        cur += size;
    }
    nextObj(cur - size) = nullptr;

    _spanMap[index].PushFront(span);

    Log::record(Log::INFO, "��ȡһ���ǿտ�ȳɹ�");
    return span;
}

size_t CentralCache::allocRangeMember(void*& start, void*& endl, size_t batchNum, size_t size, size_t index)
{
    Log::record(Log::INFO,"��ʼ���̻߳�������ڴ�");

    Span* span = getOneSpan(index, size);

    

    if (span == nullptr) {
        throw Exception(1,"���̻߳�������ڴ�ʧ�ܣ�δ��ȡ�����");
    }
    if (span->_list == nullptr) {
        throw Exception(1, "���̻߳�������ڴ�ʧ�ܣ���ȡ���յĿ��");
    }

    Log::record(Log::INFO, "��ʼ����С���ڴ�����");

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
    
    Log::record(Log::INFO, "����С���ڴ����гɹ�");

    Log::record(Log::INFO, "���̻߳�������ڴ�ɹ�");

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