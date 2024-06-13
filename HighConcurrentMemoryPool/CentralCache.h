#pragma once
#include "common.h"
#include "Log.h"
#include "Exception.h"

class CentralCache {
public:
	//…Í«Îø’º‰
	void* allocMember();
	// Õ∑≈ø’º‰
	void releaseMember();
private:
	CentralCache();
};

