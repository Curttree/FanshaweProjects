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
}

void cGameStateManager::Update(float deltaTime) {

	for (cAlien* alien : aliens) {
		alien->Update(deltaTime);
	}

	if (::g_pPlayer) {
		::g_pPlayer->Update(deltaTime);
	}
}

void cGameStateManager::InitializePlayer() {

	::g_pPlayer = new cPlayer();
	::g_pPlayer->particle->SetPosition(glm::vec3(0.f, -1200.f, 0.f));
	::g_vec_pMeshes.push_back(::g_pPlayer->mesh);
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