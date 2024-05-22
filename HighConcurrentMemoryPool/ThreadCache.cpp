#include "ThreadCache.h"

void* ThreadCache::allocMemory(size_t alloc_byte)
{
    //申请空间是否过大
    if (alloc_byte > MAX_ALLOC_SIZE) {
        throw Exception(1, "Failed ThreadCache allocMemory - allocByte greater than MAX_SIZE :" + std::to_string(alloc_byte));
    }
    size_t size = BitAlignment::RoundUp(alloc_byte);
    size_t index = BitAlignment::Index(size);
    //自由链表相应桶有空间时，分配出去
    if (_freeList[index].empty()) {
        //向中心缓存申请空间
        return nullptr;
    }
    else {
        return _freeList[index].pop();
    }
    
}

void ThreadCache::releaseMemory(void* obj, size_t obj_byte)
{
    if (obj_byte > MAX_ALLOC_SIZE) {
        throw Exception(5, "Failed ThreadCache releaseMemory - obj_byte greater than MAX_SIZE :" + std::to_string(obj_byte));
    }
    size_t index = BitAlignment::Index(obj_byte);
    _freeList[index].push(obj);
}

