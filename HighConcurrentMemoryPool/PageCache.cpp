#include "PageCache.h"

PageCache::GC PageCache::_gc;
PageCache* PageCache::_pc = PageCache::CreatePageCache();
std::mutex PageCache::_pcMtx;

PageCache::GC::~GC() {
	DestructionPageCache();
}
PageCache::~PageCache()
{
}
PageCache* PageCache::CreatePageCache()
{
	if (_pc == nullptr) {
		_pcMtx.lock();
		if (_pc == nullptr) {
			_pc = new PageCache();
		}
		_pcMtx.unlock();
	}
	return _pc;
}
void PageCache::DestructionPageCache()
{
	if (_pc != nullptr) {
		_pcMtx.lock();
		if (_pc != nullptr) {
			delete _pc;
		}
		_pcMtx.unlock();
	}
}

Span* PageCache::allocSpan(size_t index)
{
	Log::record(Log::INFO,"开始向中心缓存分配Span");
	if (index < 1 || index >= NPAGES) {
		throw Exception(1,"向PageCache申请的Span的页数不符合要求"+std::to_string(index));
	}

	// 检查PageCache中是否能分配指定页数的Span

	_mtx.lock();
	for (size_t i = index; i < NPAGES; i++) {
		if (!_spanMap[i].empty()) {
			 Span* span = _spanMap[index].PopFront();
			 // 如果为大Span先切分为小Span
			 if (i != index) {
				 sectionSpan(span, span->_n, index);
			 }
			 _mtx.unlock();
			 return span;
		}
	}
	// 向堆申请NPAGES大小的内存
	Span* span = new Span();
	void* ptr = SystemAlloc(NPAGES - 1);
	span->_pageId = (PAGE_ID)ptr >> PAGE_SHIFT;
	span->_n = NPAGES - 1;
	sectionSpan(span, span->_n, index);

	_mtx.unlock();
	Log::record(Log::INFO, "向中心缓存分配Span成功");
	return span;
}

void PageCache::sectionSpan(Span* span, size_t n, size_t newn)
{
	Log::record(Log::INFO,"切分"+std::to_string(n)+"页的Span为"+std::to_string(newn));
	Span* KSpan = new Span();

	KSpan->_pageId = span->_pageId + newn;
	KSpan->_n = n - newn;
	_spanMap[KSpan->_n].PushFront(KSpan);
	span->_n = newn;
	Log::record(Log::INFO, "切分完成");
}
PageCache::PageCache()
{
	
}

