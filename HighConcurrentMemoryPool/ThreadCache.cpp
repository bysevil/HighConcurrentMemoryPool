#include "ThreadCache.h"


ThreadCache::ThreadCache()
{
    _memoryFront = malloc(MAX_ALLOC_SIZE);
    if (_memoryFront == nullptr) {
        throw Exception(2, "Failed ThreadCache malloc ：malloc return nullptr");
    }
    _freeList = new FreeList();
    _residueByte = MAX_ALLOC_SIZE;
}

void* ThreadCache::allocMemory(size_t alloc_byte)
{
    //申请空间是否过大
    if (alloc_byte > MAX_ALLOC_SIZE) {
        throw Exception(1, "Failed ThreadCache allocMemory - allocByte greater than MAX_ALLOC_SIZE :" + std::to_string(alloc_byte));
    }
    //自由链表相应桶有空间时，分配出去
    if (false) {

    }
    // 是否需要向中心缓存申请空间

    return nullptr;
}

void ThreadCache::releaseMemory(void* obj, size_t obj_byte)
{
    
}

