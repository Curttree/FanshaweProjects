#pragma once
#include "cStructure.h"
#include "..\sVec2.h"

class cStructureFactory {
public:
	static cStructureFactory* Instance();
	cStructure* BuildStructure(int type, Vec2 position, float angle);
private:
	cStructureFactory();
	static cStructureFactory* _instance;
};