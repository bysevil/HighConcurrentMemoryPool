#include "ThreadCache.h"


ThreadCache::ThreadCache()
{
    _memoryFront = malloc(MAX_ALLOC_SIZE);
    if (_memoryFront == nullptr) {
        throw Exception(2, "Failed ThreadCache malloc ��malloc return nullptr");
    }
    _freeList = new FreeList();
    _residueByte = MAX_ALLOC_SIZE;
}

void* ThreadCache::allocMemory(size_t alloc_byte)
{
    //����ռ��Ƿ����
    if (alloc_byte > MAX_ALLOC_SIZE) {
        throw Exception(1, "Failed ThreadCache allocMemory - allocByte greater than MAX_ALLOC_SIZE :" + std::to_string(alloc_byte));
    }
    //����������ӦͰ�пռ�ʱ�������ȥ
    if (false) {

    }
    // �Ƿ���Ҫ�����Ļ�������ռ�

    return nullptr;
}

void ThreadCache::releaseMemory(void* obj, size_t obj_byte)
{
    
}

