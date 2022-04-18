#include "GameplayManager.h"
#include "globals.h"
#include "cPropFactory.h"
#include "propDefs.h"
#include "soundDefs.h"
#include <iostream>

GameplayManager::GameplayManager() {
    this->canList.clear();
}

GameplayManager::~GameplayManager() {

}

void GameplayManager::SetupScene(void) {
    cProp* can1 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(0.f, 3.f, 30.f));
    cProp* can2 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(-10.f, 3.f, 30.f));
    cProp* can3 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(-20.f, 3.f, 30.f));
    cPropFactory::Instance()->createProp(PROP_BOX, glm::vec3(20.f, 0.f, 40.f));
    cPropFactory::Instance()->createProp(PROP_BOX, glm::vec3(2.f, 0.f, 40.f));
    cPropFactory::Instance()->createProp(PROP_CITY_IMPOSTER, glm::vec3(0.f, 0.f, 200.f));
    cPropFactory::Instance()->createProp(PROP_TEST_GROUND, glm::vec3(0.f, -3.f, 50.f));

    canList.push_back(can1);
    canList.push_back(can2);
    canList.push_back(can3);
}
void GameplayManager::GameStart() {
    ::g_pGameEngine->audioManager.PlayAudio(SOUND_AMBIENCE);
}

void GameplayManager::GameOver() {
    ::g_pGameEngine->audioManager.PlayAudio(SOUND_CANS_GUY);
}

void GameplayManager::Fire() {

    //cPropFactory::Instance()->createProp(PROP_BULLET, ::g_pFlyCamera->getAt()+::g_pFlyCamera->getCameraDirection() * 5.f);
    if (canList.size() > 0) {
        bool hit = false;
        for (unsigned int index = 0; index < canList.size(); index++) {
            float dist = glm::distance(::g_pFlyCamera->getEye(), canList[index]->mesh->positionXYZ);
            glm::vec3 point = (dist * glm::normalize(::g_pFlyCamera->getAtDirection())) + ::g_pFlyCamera->getEye();
            float distance = glm::distance(canList[index]->mesh->positionXYZ, point);
            if (distance < 1.f) {
                ::g_pGameEngine->audioManager.PlayAudio(SOUND_PING);
                canList[index]->rigidBody->ApplyImpulseAtPoint((point - canList[index]->mesh->positionXYZ ) * 0.5f, point);
                canList[index]->Destroy(3.f);
                canList.erase(canList.begin()+index);
                if (canList.size() == 0) {
                    GameOver();
                }
                //Exit early.
                return;
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