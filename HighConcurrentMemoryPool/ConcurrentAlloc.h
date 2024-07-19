#pragma once
#include"ThreadCache.h"

void* ConcurrentAlloc(size_t size) {
    //���threadCache�����ڣ��򴴽�
    if (TLS_tc == nullptr) {
        TLS_tc = new ThreadCache();
        if (TLS_tc == nullptr) {
            throw Exception(5, "Failed CreateThreadCache - malloc nullptr");
        }
    }

    return TLS_tc->allocMemory(size);
}

void ConcurrentFree(void* obj,size_t obj_byte) {
    TLS_tc->releaseMemory(obj, obj_byte);
}