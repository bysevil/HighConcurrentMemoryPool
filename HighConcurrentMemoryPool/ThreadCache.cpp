#include "ThreadCache.h"
#include "CentralCache.h"

void* ThreadCache::allocMemory(size_t alloc_byte)
{
    Log::record(Log::INFO,"开始向对象分配内存:"+ std::to_string(alloc_byte));
    
    //申请空间是否过大，过大时报异常
    if (alloc_byte > MAX_SIZE) {
        throw Exception(1, "分配内存失败 - 申请的大小过大:" + std::to_string(alloc_byte));
    }

    //计算对齐数
    size_t size = BitAlignment::RoundUp(alloc_byte);
    //计算桶下标
    size_t index = BitAlignment::Index(size);

    void* ret = nullptr;
    if (_freeMap[index].empty()) {
        //自由链表没有空间，向中心缓存申请空间
        FetchFromCentralCache(index, size);
        //_freeMap[index].push(malloc(size));
    }

    //将自由链表中的空间分配出去
    ret = _freeMap[index].pop();

    Log::record(Log::INFO,"向对象分配内存成功");

    return ret;
}

void ThreadCache::releaseMemory(void* obj, size_t obj_byte)
{
    Log::record(Log::INFO, "对象向threadCache还回内存，地址：" + std::to_string((int)obj) + " 大小：" + std::to_string(BitAlignment::RoundUp(obj_byte)));
    std::thread::id thread_id = std::this_thread::get_id();
    
    //对象是否过大
    if (obj_byte > MAX_SIZE) {
        throw Exception(5, "对象还回的内存过大:" + std::to_string(BitAlignment::RoundUp(obj_byte)));
    }
    //对象是否为空
    if (obj == nullptr) {
        throw Exception(6, "对象为nullptr，还回内存失败");
    }
    
    //计算桶下标
    size_t index = BitAlignment::Index(obj_byte);

    //空间返回给自由链表
    _freeMap[index].push(obj);

    Log::record(Log::INFO,"对象向threadCache还回内存成功");
}

size_t ThreadCache::bathItemNum(size_t index,size_t size) {
    Log::record(Log::INFO, "计算需要申请的小块内存的个数");
    size_t ret = min(_freeMap[index].getMaxSize(),BitAlignment::MaxAllocItem(size));
    if (ret == _freeMap[index].getMaxSize()) {
        _freeMap[index].setMaxSize();
    }
    Log::record(Log::INFO, "计算需要申请的小块内存的个数成功："+std::to_string(ret));
    return ret;
}

void ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
    Log::record(Log::INFO, "向CentralCache申请指定数量的小块内存");
    size_t bathNum = bathItemNum(index,size);
     
    void* start = nullptr;
    void* endl = nullptr;
    size_t actualNum = CentralCache::CreateCentralCache()->allocRangeMember(start,endl, bathNum, size, index);

    _freeMap[index].pushRange(start, endl);
    Log::record(Log::INFO, "向CentralCache申请指定数量的小块内存成功");
}





