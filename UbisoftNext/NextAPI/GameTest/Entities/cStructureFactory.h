#pragma once
#include "cGameEntity.h"
#include "..\sVec2.h"

class cStructureFactory {
public:
	static cStructureFactory* Instance();
	cGameEntity* BuildStructure(int type, Vec2 position, float angle);
private:
	cStructureFactory();
	static cStructureFactory* _instance;
};