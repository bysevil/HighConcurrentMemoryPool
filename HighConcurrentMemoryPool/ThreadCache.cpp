#include "ThreadCache.h"

#ifdef _WIN32
__declspec(thread) ThreadCache* TLS_tc = nullptr;
#else
_thread ThreadCache* TLS_tc = nullptr;
#endif

void* ThreadCache::allocMemory(size_t alloc_byte)
{
    Log::record("execute allocMemory");
    //申请空间是否过大
    if (alloc_byte > MAX_SIZE) {
        throw Exception(1, "Failed ThreadCache allocMemory - allocByte greater than MAX_SIZE :" + std::to_string(alloc_byte));
    }
    size_t size = BitAlignment::RoundUp(alloc_byte);
    size_t index = BitAlignment::Index(size);
    //自由链表相应桶有空间时，分配出去
    void* ret = nullptr;
    if (_freeList[index].empty()) {
        //向中心缓存申请空间
    }
    else {
        ret = _freeList[index].pop();
    }
    Log::record("success allocMemory");
    return ret;
}

void ThreadCache::releaseMemory(void* obj, size_t obj_byte)
{
    Log::record("execute releaseMemory: pointer" + std::to_string((int)obj) + " size" + std::to_string(obj_byte));
    std::thread::id thread_id = std::this_thread::get_id();
    
    if (obj_byte > MAX_SIZE) {
        throw Exception(5, "Failed ThreadCache releaseMemory - obj_byte greater than MAX_SIZE :" + std::to_string(obj_byte));
    }

    
    size_t index = BitAlignment::Index(obj_byte);
    _freeList[index].push(obj);
    Log::record("success releaseMemory");
}

ThreadCache* CreateThreadCache()
{
    if (TLS_tc == nullptr) {
        TLS_tc = new ThreadCache();
    }
    if (TLS_tc == nullptr) {
        throw Exception(5,"Failed CreateThreadCache - malloc nullptr");
    }
    return TLS_tc;
}

void DestructionThreadCache()
{
    if (TLS_tc != nullptr) {
        delete TLS_tc;
        TLS_tc = nullptr;
    }
}


