#pragma once
#include "cProjectile.h"
#include "..\sVec2.h"

class cProjectileFactory {
public:
	static cProjectileFactory* Instance();
	cProjectile* CreateProjectile(int type, Vec2 position, Vec2 velocity);
private:
	cProjectileFactory();
	static cProjectileFactory* _instance;
};