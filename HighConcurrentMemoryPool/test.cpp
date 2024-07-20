#include"concurrentAlloc.h"

//测试threadCache是否正常运行
void test1() {
	try {
		//Log::setOutLV(Log::DEBUG);
		for(int i = 0; i < 10; i++) {
			void* tmp = ConcurrentAlloc(18);
			ConcurrentFree(tmp,18);
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	catch (Exception E) {
		Log::record(Log::LOG_ERROR,"[ID:" + std::to_string(E.geterrid()) + "] " + E.geterrmsg());
		exit(0);
	}
}

int main() {
	std::thread t1(test1);
	t1.join();
	//std::thread t2(test1);
	//t2.join();
	return 0;
}
