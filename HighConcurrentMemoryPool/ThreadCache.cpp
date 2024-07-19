#include "ThreadCache.h"
#include "CentralCache.h"

void* ThreadCache::allocMemory(size_t alloc_byte)
{
    Log::record(Log::INFO,"execute allocMemory");
    
    //����ռ��Ƿ���󣬹���ʱ���쳣
    if (alloc_byte > MAX_SIZE) {
        throw Exception(1, "Failed ThreadCache allocMemory - allocByte greater than MAX_SIZE :" + std::to_string(alloc_byte));
    }

    //���������
    size_t size = BitAlignment::RoundUp(alloc_byte);
    //����Ͱ�±�
    size_t index = BitAlignment::Index(size);


    void* ret = nullptr;
    if (_freeMap[index].empty()) {
        //��������û�пռ䣬�����Ļ�������ռ�
       //_freeMap[index].push(FatchFromCentralCache());
        //_freeMap[index].push(malloc(size));
    }

    //�����������еĿռ�����ȥ
    ret = _freeMap[index].pop();

    Log::record(Log::INFO,"success allocMemory");

    return ret;
}

void ThreadCache::releaseMemory(void* obj, size_t obj_byte)
{
    Log::record(Log::INFO, "execute releaseMemory: pointer" + std::to_string((int)obj) + " size" + std::to_string(BitAlignment::RoundUp(obj_byte)));
    std::thread::id thread_id = std::this_thread::get_id();
    
    //�����Ƿ����
    if (obj_byte > MAX_SIZE) {
        throw Exception(5, "Failed ThreadCache releaseMemory - obj_byte greater than MAX_SIZE :" + std::to_string(BitAlignment::RoundUp(obj_byte)));
    }
    //�����Ƿ�Ϊ��
    if (obj == nullptr) {
        throw Exception(6, "Fail ThreadCache releaseMemory - push obj == nullptr");
    }
    
    //����Ͱ�±�
    size_t index = BitAlignment::Index(obj_byte);

    //�ռ䷵�ظ���������
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





