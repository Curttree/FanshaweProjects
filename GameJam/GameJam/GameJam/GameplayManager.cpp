#include "GameplayManager.h"
#include "globals.h"
#include <iostream>

GameplayManager::GameplayManager() {
    this->canList.clear();
}

GameplayManager::~GameplayManager() {

}

void GameplayManager::SetupScene(void) {
    cEntity* can = ::g_pGameEngine->entityManager.CreateEntity();
    cMesh* can_mesh = new cMesh();
    can_mesh->meshName = "Blobby.fbx";
    can_mesh->scale = glm::vec3(1.0f);
    can_mesh->positionXYZ = glm::vec3(0.f, 0.f, 10.f);
    can_mesh->textureNames[0] = "BrainNerve.bmp";
    can_mesh->textureRatios[0] = 1.f;
    can->mesh = can_mesh;
    can->position = can_mesh->positionXYZ;
    can->scale = can_mesh->scale;
    can->rotation = can_mesh->orientationXYZ;
    this->canList.push_back(can);
    ::g_vec_pMeshes.push_back(can->mesh);

}
void GameplayManager::GameStart() {

}

void GameplayManager::GameOver() {

}

void GameplayManager::SetAiming(bool _aiming) {
    aiming = _aiming;
}

bool GameplayManager::GetAiming() {
    return aiming;
}