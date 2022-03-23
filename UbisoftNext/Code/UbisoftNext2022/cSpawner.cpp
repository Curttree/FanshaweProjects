#include "stdafx.h"
#include "cSpawner.h"
#include "Entities/cStructureFactory.h"
#include "cWorldSpace.h"
#include "globalFunctions.h"

cSpawner::cSpawner(cPlanet* _planet) {
	planet = _planet;
}
cSpawner::~cSpawner() {

}

void cSpawner::Init() {
	for (unsigned int x = 0; x < 5; x++) {
		SpawnEnemy();
	}
	for (unsigned int x = 0; x < 2; x++) {
		SpawnCollectible();
	}
}

void cSpawner::Update(float deltaTime) {
	collectibleTime += deltaTime;
	if (collectibleTime > collectibleTimer) {
		SpawnCollectible();
	}

	enemyTime += deltaTime;
	if (enemyTime > enemyTimer) {
		SpawnEnemy();
	}
}

void cSpawner::SpawnCollectible() {
	Vec2 spawnPosition;
	float spawnAngle;
	planet->GetSurfacePosition(spawnPosition, spawnAngle);
	collectibleTime = 0.f;

	cStructureFactory* structFactory = cStructureFactory::Instance();

	int type = 0;
	if (cWorldSpace::Instance()->gameState->GetFuel() > 500.f) {
		type = gGetRandBetween(0, 2);
	}
	cStructure* newStructure = structFactory->BuildStructure(type, spawnPosition, spawnAngle);

	cWorldSpace* world = cWorldSpace::Instance();
	world->structures.push_back(newStructure);
}

void cSpawner::SpawnEnemy() {
	Vec2 spawnPosition;
	float spawnAngle;
	planet->GetSurfacePosition(spawnPosition, spawnAngle);
	enemyTime = 0.f;

	cStructureFactory* structFactory = cStructureFactory::Instance();

	int type = gGetRandBetween(2, 4);
	cStructure* newStructure = structFactory->BuildStructure(type, spawnPosition, spawnAngle);

	cWorldSpace* world = cWorldSpace::Instance();
	world->structures.push_back(newStructure);
}