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
			alien->DelaySinceLastMove(y * 0.2f);
			alien->particle->SetPosition(spawnPosition);
			alien->mesh->positionXYZ = spawnPosition;
			spawnPosition.x -= step;
			::g_vec_pMeshes.push_back(alien->mesh);
			aliens.push_back(alien);
			livingAliens.push_back(alien);
		}
		spawnPosition.y += step;
		spawnPosition.x = 750.f;
	}

	ufo = new cUfo();
	::g_vec_pMeshes.push_back(ufo->mesh);

	missile = new cAlienMissile();
	::g_vec_pMeshes.push_back(missile->mesh);
}

void cGameStateManager::Update(float deltaTime) {
	if (playerIsAlive && gameHasStarted) {
		for (cAlien* alien : aliens) {
			alien->Update(deltaTime);
		}

		if (::g_pPlayer) {
			::g_pPlayer->Update(deltaTime);
		}

		if (ufo) {
			ufo->Update(deltaTime);
		}

		if (missile) {
			missile->Update(deltaTime);
		}
	}
	else {
		gameOverCounter += deltaTime;
		if (gameOverCounter >= gameOverTime) {
			gameHasEnded = true;
		}
	}

	if (gameHasStarted) {
		currentUfoCycleTime += deltaTime;
		if (currentUfoCycleTime >= ufoSpawnTime) {
			ufo->StartMoving();
			currentUfoCycleTime = 0.f;
		}
		currentMissileCycleTime += deltaTime;
		if (currentMissileCycleTime >= missileSpawnTime && livingAliens.size() > 0) {
			int alienIndex = ::gGetRandBetween(0, (int)livingAliens.size());
			missile->StartMoving(livingAliens[alienIndex]->particle->GetPosition());
			currentMissileCycleTime = 0.f;
		}
		hasMovedDown = false;
	}
}

void cGameStateManager::InitializePlayer() {

	::g_pPlayer = new cPlayer();
	::g_pPlayer->particle->SetPosition(glm::vec3(0.f, -1050.f, 0.f));
	::g_pPlayer->mesh->positionXYZ = glm::vec3(0.f, -1050.f, 0.f);
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

	if (theMessage.command == "Reverse Ships" && !hasMovedDown) {
		hasMovedDown = true;
		for (cAlien* alien : aliens) {
			alien->RecieveMessage(theMessage);
		}

	}
	if (theMessage.command == "Speed Up Ships") {
		hasMovedDown = true;
		for (cAlien* alien : aliens) {
			alien->RecieveMessage(theMessage);
		}
		// Clean up our list of living aliens.
		cAlien* deadAlien = (cAlien*)theMessage.vec_voidPData[0];
		std::vector<cAlien*>::iterator it = std::find(livingAliens.begin(), livingAliens.end(), deadAlien);
		if (it != livingAliens.end()) {
			livingAliens.erase(it);
		}
		if (livingAliens.size() == 0) {
			playerIsAlive = false;
		}
	}
	if (theMessage.command == "Game Over") {
		gameHasStarted = false;
		gameHasEnded = true;
	}
	if (theMessage.command == "Player Died") {
		playerIsAlive = false;
	}
	return true;
}
bool cGameStateManager::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	return true;
}
bool cGameStateManager::SetReciever(iMediator* pTheReciever) {
	return true;
}

bool cGameStateManager::GameStarted() {
	return gameHasStarted;
}
void cGameStateManager::StartGame() {
	gameHasStarted = true;
}

bool cGameStateManager::GameEnded() {
	return gameHasEnded;
}