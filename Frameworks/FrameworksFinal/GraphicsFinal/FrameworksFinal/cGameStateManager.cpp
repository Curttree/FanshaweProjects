#include "cGameStateManager.h"
#include "globals.h"

void cGameStateManager::AssembleAliens() {
	glm::vec3 startPosition = glm::vec3(750.f, 300.f, 0.f);
	std::vector<int> types = {3,3,1,1,2};
	float step = 150.f;
	glm::vec3 spawnPosition = startPosition;
	for (int y = 0; y < types.size(); y++) {
		for (int x = 0; x < 11; x++) {
			cAlien* alien = new cAlien(types[y]);
			alien->particle->SetPosition(spawnPosition);
			spawnPosition.x -= step;
			::g_vec_pMeshes.push_back(alien->mesh);
			aliens.push_back(alien);
		}
		spawnPosition.y += step;
		spawnPosition.x = 750.f;
	}

	ufo = new cUfo();
	::g_vec_pMeshes.push_back(ufo->mesh);
}

void cGameStateManager::Update(float deltaTime) {

	for (cAlien* alien : aliens) {
		alien->Update(deltaTime);
	}

	if (::g_pPlayer) {
		::g_pPlayer->Update(deltaTime);
	}

	if (ufo) {
		ufo->Update(deltaTime);
	}

	currentUfoCycleTime += deltaTime;
	if (currentUfoCycleTime >= ufoSpawnTime) {
		ufo->StartMoving();
		currentUfoCycleTime = 0.f;
	}
}

void cGameStateManager::InitializePlayer() {

	::g_pPlayer = new cPlayer();
	::g_pPlayer->particle->SetPosition(glm::vec3(0.f, -1050.f, 0.f));
	::g_vec_pMeshes.push_back(::g_pPlayer->mesh);
	if (::g_pPlayer->bullet->mesh) {
		::g_vec_pMeshes.push_back(::g_pPlayer->bullet->mesh);
	}
}

void cGameStateManager::BuildShields() {
	float startingPoint = -600.f;
	for (int x = 0; x < 3; x++) {
		BuildAShield(x * 480.f + startingPoint);
	}
}

void cGameStateManager::BuildAShield(float x_offset) {
	float startHeight = -800.f;
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 4; x++) {
			cShieldPixel* shield = new cShieldPixel(glm::vec3(20.f * x + x_offset, startHeight + 10.f * y * 2, 0.f));
			::g_vec_pMeshes.push_back(shield->mesh);
		}
		for (int x = 9; x < 13; x++) {
			cShieldPixel* shield = new cShieldPixel(glm::vec3(20.f * x + x_offset, startHeight + 10.f * y * 2, 0.f));
			::g_vec_pMeshes.push_back(shield->mesh);
		}
	}

	for (int x = 0; x < 5; x++) {
		cShieldPixel* shield = new cShieldPixel(glm::vec3(20.f * x + x_offset, startHeight + 60.f, 0.f));
		::g_vec_pMeshes.push_back(shield->mesh);
	}
	for (int x = 8; x < 13; x++) {
		cShieldPixel* shield = new cShieldPixel(glm::vec3(20.f * x + x_offset, startHeight + 60.f, 0.f));
		::g_vec_pMeshes.push_back(shield->mesh);
	}

	for (int y = 4; y < 7; y++) {
		for (int x = 0; x < 13; x++) {
			cShieldPixel* shield = new cShieldPixel(glm::vec3(20.f * x + x_offset, startHeight + 10.f * y * 2, 0.f));
			::g_vec_pMeshes.push_back(shield->mesh);
		}
	}

	for (int x = 1; x < 12; x++) {
		cShieldPixel* shield = new cShieldPixel(glm::vec3(20.f * x + x_offset, startHeight + 140.f, 0.f));
		::g_vec_pMeshes.push_back(shield->mesh);
	}

	for (int x = 2; x < 11; x++) {
		cShieldPixel* shield = new cShieldPixel(glm::vec3(20.f * x + x_offset, startHeight + 160.f, 0.f));
		::g_vec_pMeshes.push_back(shield->mesh);
	}

	for (int x = 3; x < 10; x++) {
		cShieldPixel* shield = new cShieldPixel(glm::vec3(20.f * x + x_offset, startHeight + 180.f, 0.f));
		::g_vec_pMeshes.push_back(shield->mesh);
	}
}


bool cGameStateManager::RecieveMessage(sMessage theMessage) {
	return true;
}
bool cGameStateManager::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	return true;
}
bool cGameStateManager::SetReciever(iMediator* pTheReciever) {
	return true;
}