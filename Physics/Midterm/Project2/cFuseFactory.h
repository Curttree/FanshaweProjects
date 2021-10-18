#pragma once
#include "iFuse.h"

class cFuseFactory {
public:
	static cFuseFactory* Instance();
	iFuse* createFuse(int type, bool* hitApex, float maxTime = 120.f);
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
private:
	cFuseFactory();
	static cFuseFactory* _instance;
};