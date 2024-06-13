#include"ThreadCache.h"


void test() {
	try {
		//Log::setOutLV(Log::DEBUG);
		ThreadCache* TLS_tc = CreateThreadCache();
		for(int i = 0; i < 10; i++) {
			void* tmp = TLS_tc->allocMemory(222222222);
			TLS_tc->releaseMemory(tmp, 18);
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		DestructionThreadCache();
		TLS_tc = nullptr;
	}
	catch (Exception E) {
		Log::record(Log::ERROR, "[id:" + std::to_string(E.geterrid()) + "] " + E.geterrmsg());
	}
}

int main() {
	std::thread t1(test);
	t1.join();
	return 0;
}
