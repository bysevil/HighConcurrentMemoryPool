#include "CentralCache.h"

std::mutex CentralCache::_mtx;
CentralCache* CentralCache::_cc = CentralCache::CreateCentralCache();

CentralCache::GC::GC()
{}

CentralCache::GC::~GC() {
    DestructionCentralCache();
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

void* CentralCache::allocMember(int index)
{
    return nullptr;
}