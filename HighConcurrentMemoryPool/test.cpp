#include"ThreadCache.h"


void test() {
	ThreadCache* TLS_tc = CreateThreadCache();
	for(int i = 0; i < 10; i++) {
		void* tmp = TLS_tc->allocMemory(6);
		TLS_tc->releaseMemory(tmp, 6);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	DestructionThreadCache();
	TLS_tc = nullptr;
}

int main() {
	try {
		std::thread t1(test);
		//std::thread t2(test);

		t1.join();
		//t2.join();
	}catch(...){}
	return 0;
}
