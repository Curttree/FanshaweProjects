#include "stdafx.h"
#include "cSpawner.h"
#include "Entities/cStructureFactory.h"
#include "cWorldSpace.h"

cSpawner::cSpawner(cPlanet* _planet) {
	planet = _planet;
}
cSpawner::~cSpawner() {

}

void cSpawner::Update(float deltaTime) {
	collectibleTime += deltaTime;
	if (collectibleTime > collectibleTimer) {
		Vec2 spawnPosition;
		float spawnAngle;
		planet->GetSurfacePosition(spawnPosition, spawnAngle);
		collectibleTime = 0.f;

		cStructureFactory* structFactory = cStructureFactory::Instance();
		cGameEntity* newStructure = structFactory->BuildStructure(0, spawnPosition, spawnAngle);

		cWorldSpace* world = cWorldSpace::Instance();
		world->structures.push_back(newStructure);

	}
}