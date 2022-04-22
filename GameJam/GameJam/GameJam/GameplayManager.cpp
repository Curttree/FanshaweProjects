#include "GameplayManager.h"
#include "globals.h"
#include "cPropFactory.h"
#include "cParticleFactory.h"
#include "propDefs.h"
#include "particleDefs.h"
#include "soundDefs.h"
#include <iostream>

GameplayManager::GameplayManager() {
    this->canList.clear();
}

GameplayManager::~GameplayManager() {

}

void GameplayManager::SetupScene(void) {
    cProp* can1 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(0.f, 3.f, 230.f));
    cProp* can2 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(-22.f, 3.f, 66.f),glm::vec3(0.f,glm::pi<float>()/2.f,0.f));
    cProp* can3 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(13.5f, 4.f, 140.f), glm::vec3(0.f, glm::pi<float>() / 5.f, 0.f));
    cProp* can4 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(13.2f, 2.f, 39.8f));
    cPropFactory::Instance()->createProp(PROP_BOX, glm::vec3(13.6f, 0.f, 40.f));
    cPropFactory::Instance()->createProp(PROP_BOX, glm::vec3(15.f, 0.f, 35.f), glm::vec3(0.f, glm::pi<float>() / 7.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_HOTDOG_STAND, glm::vec3(14.f, 0.f, 140.f),glm::vec3(0.f,glm::pi<float>() / 2.f,0.f));
    cPropFactory::Instance()->createProp(PROP_BARRIER, glm::vec3(15.f, 0.f, 380.f));
    cPropFactory::Instance()->createProp(PROP_BARRIER, glm::vec3(5.f, 0.f, 380.f));
    cPropFactory::Instance()->createProp(PROP_BARRIER, glm::vec3(-5.f, 0.f, 380.f));
    cPropFactory::Instance()->createProp(PROP_BARRIER, glm::vec3(-15.f, 0.f, 380.f));
    cPropFactory::Instance()->createProp(PROP_CITY_IMPOSTER, glm::vec3(0.f, 20.f, 200.f));
    cPropFactory::Instance()->createProp(PROP_TEST_GROUND, glm::vec3(0.f, -3.5f, 50.f));

    cParticleFactory::Instance()->createParticle(PARTICLE_TEST, glm::vec3(0.f,10.f,40.f), false);

    canList.push_back(can1);
    canList.push_back(can2);
    canList.push_back(can3);
    canList.push_back(can4);
}
void GameplayManager::GameStart() {
    ::g_pGameEngine->audioManager.PlayAudio(SOUND_AMBIENCE);
}

void GameplayManager::GameOver() {
    ::g_pGameEngine->audioManager.PlayAudio(SOUND_CANS_GUY);
}

void GameplayManager::Fire() {
    glm::vec3 end = (1000.f * glm::normalize(::g_pFlyCamera->getAtDirection())) + ::g_pFlyCamera->getEye();
    gdp2022Physics::iCollisionBody* rayHit = ::g_pGameEngine->m_PhysicsWorld->RayHit(::g_pFlyCamera->getEye(), end);
    //cPropFactory::Instance()->createProp(PROP_BULLET, ::g_pFlyCamera->getAt()+::g_pFlyCamera->getCameraDirection() * 5.f);
    if (canList.size() > 0) {
        bool hit = false;
        for (unsigned int index = 0; index < canList.size(); index++) {
            if (rayHit == canList[index]->rigidBody) {
                float dist = glm::distance(::g_pFlyCamera->getEye(), canList[index]->mesh->positionXYZ);
                glm::vec3 point = (dist * glm::normalize(::g_pFlyCamera->getAtDirection())) + ::g_pFlyCamera->getEye();
                float distance = glm::distance(canList[index]->mesh->positionXYZ, point);
                if (distance < 1.f) {
                    ::g_pGameEngine->audioManager.PlayAudio(SOUND_PING);
                    canList[index]->rigidBody->ApplyImpulseAtPoint((canList[index]->mesh->positionXYZ - point) * 0.5f, point);
                    canList[index]->Destroy(3.f);
                    canList.erase(canList.begin() + index);
                    if (canList.size() == 0) {
                        GameOver();
                    }
                    //Exit early.
                    return;
                }
            }
        }
        if (!hit) 
        {
            ::g_pGameEngine->audioManager.PlayAudio(SOUND_GUN);
        }
    }
    else {
        ::g_pGameEngine->audioManager.PlayAudio(SOUND_GUN);
    }
}

void GameplayManager::SetAiming(bool _aiming) {
    aiming = _aiming;
    if (!aiming) {
        //Reset the camera.
        ::g_pFlyCamera->Reset();
    }
}

bool GameplayManager::GetAiming() {
    return aiming;
}