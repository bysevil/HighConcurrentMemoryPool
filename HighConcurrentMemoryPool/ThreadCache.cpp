#include "ThreadCache.h"
#include "CentralCache.h"

void* ThreadCache::allocMemory(size_t alloc_byte)
{
    Log::record(Log::INFO,"��ʼ���������ڴ�:"+ std::to_string(alloc_byte));
    
    //����ռ��Ƿ���󣬹���ʱ���쳣
    if (alloc_byte > MAX_SIZE) {
        throw Exception(1, "�����ڴ�ʧ�� - ����Ĵ�С����:" + std::to_string(alloc_byte));
    }

    //���������
    size_t size = BitAlignment::RoundUp(alloc_byte);
    //����Ͱ�±�
    size_t index = BitAlignment::Index(size);

    void* ret = nullptr;
    if (_freeMap[index].empty()) {
        //��������û�пռ䣬�����Ļ�������ռ�
        FetchFromCentralCache(index, size);
        //_freeMap[index].push(malloc(size));
    }

    //�����������еĿռ�����ȥ
    ret = _freeMap[index].pop();

    Log::record(Log::INFO,"���������ڴ�ɹ�");

    return ret;
}

void ThreadCache::releaseMemory(void* obj, size_t obj_byte)
{
    Log::record(Log::INFO, "������threadCache�����ڴ棬��ַ��" + std::to_string((int)obj) + " ��С��" + std::to_string(BitAlignment::RoundUp(obj_byte)));
    std::thread::id thread_id = std::this_thread::get_id();
    
    //�����Ƿ����
    if (obj_byte > MAX_SIZE) {
        throw Exception(5, "���󻹻ص��ڴ����:" + std::to_string(BitAlignment::RoundUp(obj_byte)));
    }
    //�����Ƿ�Ϊ��
    if (obj == nullptr) {
        throw Exception(6, "����Ϊnullptr�������ڴ�ʧ��");
    }
    
    //����Ͱ�±�
    size_t index = BitAlignment::Index(obj_byte);

    //�ռ䷵�ظ���������
    _freeMap[index].push(obj);

    Log::record(Log::INFO,"������threadCache�����ڴ�ɹ�");
}

size_t ThreadCache::bathItemNum(size_t index,size_t size) {
    Log::record(Log::INFO, "������Ҫ�����С���ڴ�ĸ���");
    size_t ret = min(_freeMap[index].getMaxSize(),BitAlignment::MaxAllocItem(size));
    if (ret == _freeMap[index].getMaxSize()) {
        _freeMap[index].setMaxSize();
    }
    Log::record(Log::INFO, "������Ҫ�����С���ڴ�ĸ����ɹ���"+std::to_string(ret));
    return ret;
}

void ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
    Log::record(Log::INFO, "��CentralCache����ָ��������С���ڴ�");
    size_t bathNum = bathItemNum(index,size);
     
    void* start = nullptr;
    void* endl = nullptr;
    size_t actualNum = CentralCache::CreateCentralCache()->allocRangeMember(start,endl, bathNum, size, index);

    _freeMap[index].pushRange(start, endl);
    Log::record(Log::INFO, "��CentralCache����ָ��������С���ڴ�ɹ�");
}





