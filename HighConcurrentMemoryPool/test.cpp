#include"ThreadCache.h"
#include <chrono>
#include <ctime>

//Exception��Log�๦�ܲ���
void test() {
	try {
		threadCache->allocMemory(MAX_ALLOC_SIZE + 1);
	}
	catch(Exception e){

	}
}

int main() {
	test();
	return 0;
}
