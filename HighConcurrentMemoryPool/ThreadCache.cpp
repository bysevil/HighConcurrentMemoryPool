#include "ThreadCache.h"
#include "CentralCache.h"

void* ThreadCache::allocMemory(size_t alloc_byte)
{
    Log::record(Log::INFO,"execute allocMemory");
    
    //申请空间是否过大，过大时报异常
    if (alloc_byte > MAX_SIZE) {
        throw Exception(1, "Failed ThreadCache allocMemory - allocByte greater than MAX_SIZE :" + std::to_string(alloc_byte));
    }

    //计算对齐数
    size_t size = BitAlignment::RoundUp(alloc_byte);
    //计算桶下标
    size_t index = BitAlignment::Index(size);


    void* ret = nullptr;
    if (_freeMap[index].empty()) {
        //自由链表没有空间，向中心缓存申请空间
       //_freeMap[index].push(FatchFromCentralCache());
        //_freeMap[index].push(malloc(size));
    }

    //将自由链表中的空间分配出去
    ret = _freeMap[index].pop();

    Log::record(Log::INFO,"success allocMemory");

    return ret;
}

void ThreadCache::releaseMemory(void* obj, size_t obj_byte)
{
    Log::record(Log::INFO, "execute releaseMemory: pointer" + std::to_string((int)obj) + " size" + std::to_string(BitAlignment::RoundUp(obj_byte)));
    std::thread::id thread_id = std::this_thread::get_id();
    
    //对象是否过大
    if (obj_byte > MAX_SIZE) {
        throw Exception(5, "Failed ThreadCache releaseMemory - obj_byte greater than MAX_SIZE :" + std::to_string(BitAlignment::RoundUp(obj_byte)));
    }
    //对象是否为空
    if (obj == nullptr) {
        throw Exception(6, "Fail ThreadCache releaseMemory - push obj == nullptr");
    }
    
    //计算桶下标
    size_t index = BitAlignment::Index(obj_byte);

    //空间返回给自由链表
    _freeMap[index].push(obj);

    Log::record(Log::INFO,"success releaseMemory");
}

size_t ThreadCache::bathItemNum(size_t index,size_t size) {
    size_t ret = std::min(_freeMap[index].getMaxSize(),BitAlignment::MaxAllocItem(size));
    if (ret == _freeMap[index].getMaxSize()) {
        _freeMap[index].setMaxSize();
    }
    return ret;
}

void ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
    size_t bathNum = bathItemNum(index,size);
     
    size_t  actualNum = CentralCache::CreateCentralCache()->allocMember();
}





