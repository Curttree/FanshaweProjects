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
    cProp* can1 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(13.2f, 1.f, 39.8f));
    cProp* can2 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(-22.f, -1.f, 66.f),glm::vec3(0.f,glm::pi<float>()/2.f,0.f));
    cProp* can3 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(13.5f, 3.f, 140.f), glm::vec3(0.f, glm::pi<float>() / 5.f, 0.f));
    cProp* can4 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(15.f, -1.f, 210.f), glm::vec3(0.f, -glm::pi<float>() / 6.f, 0.f));
    cProp* can5 = cPropFactory::Instance()->createProp(PROP_CAN, glm::vec3(-13.f, 4.f, 230.f));
    cPropFactory::Instance()->createProp(PROP_BOX, glm::vec3(13.6f, -3.f, 40.f));
    cPropFactory::Instance()->createProp(PROP_BOX, glm::vec3(15.f, -3.f, 35.f), glm::vec3(0.f, glm::pi<float>() / 7.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_BOX, glm::vec3(13.6f, -3.f, 300.f), glm::vec3(0.f, glm::pi<float>() / 4.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_BOX, glm::vec3(12.f, -3.f, 285.f), glm::vec3(0.f, -glm::pi<float>() / 5.f, 0.f));

    cPropFactory::Instance()->createProp(PROP_HOTDOG_STAND, glm::vec3(14.f, -1.f, 140.f),glm::vec3(0.f,glm::pi<float>() / 2.f,0.f));

    cPropFactory::Instance()->createProp(PROP_LIGHT, glm::vec3(15.f, 0.f, 0.f), glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_LIGHT, glm::vec3(15.f, 0.f, 200.f), glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_LIGHT, glm::vec3(15.f, 0.f, 400.f), glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_LIGHT, glm::vec3(-15.f, 0.f, 100.f), glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_LIGHT, glm::vec3(-15.f, 0.f, 300.f), glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));

    cPropFactory::Instance()->createProp(PROP_PARKING, glm::vec3(13.f, 0.f, 50.f), glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_PARKING, glm::vec3(-13.f, 0.f, 250.f), glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_PARKING, glm::vec3(-13.f, 0.f, -50.f), glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f));

    cPropFactory::Instance()->createProp(PROP_PLANTER, glm::vec3(-21.f, 0.f, 49.5f), glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_PLANTER, glm::vec3(-21.f, 0.f, 150.f), glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_PLANTER, glm::vec3(-21.f, 0.f, 349.f), glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_PLANTER, glm::vec3(21.5f, 0.f, 106.f), glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f));
    cPropFactory::Instance()->createProp(PROP_PLANTER, glm::vec3(21.5f, 0.f, 230.f), glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f));


    cPropFactory::Instance()->createProp(PROP_BENCH, glm::vec3(-13.f, 0.f, 230.f), glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f));

    cPropFactory::Instance()->createProp(PROP_BARRIER, glm::vec3(15.f, -3.f, 380.f));
    cPropFactory::Instance()->createProp(PROP_BARRIER, glm::vec3(5.f, -3.f, 380.f));
    cPropFactory::Instance()->createProp(PROP_BARRIER, glm::vec3(-5.f, -3.f, 380.f));
    cPropFactory::Instance()->createProp(PROP_BARRIER, glm::vec3(-15.f, -3.f, 380.f));

    cPropFactory::Instance()->createProp(PROP_CITY_IMPOSTER, glm::vec3(0.f, 17.f, 450.f));

    canList.push_back(can1);
    canList.push_back(can2);
    canList.push_back(can3);
    canList.push_back(can4);
    canList.push_back(can5);

    //Set level boundaries.
    {
        gdp2022Physics::iRigidBody* rigidBody = 0;
        gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(0, glm::vec3(0.f, 1.f, 0.f));
        gdp2022Physics::cRigidBodyDesc planeDesc;
        planeDesc.linearDamping = 0.f;
        planeDesc.isStatic = true;
        planeDesc.mass = 0.f;
        planeDesc.rotation = glm::quat(glm::vec3(0.f));
        planeDesc.position = glm::vec3(0.f, -3.f, 0.f);
        planeDesc.linearVelocity = glm::vec3(0.f);

        rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);
        if (rigidBody != 0 && ::g_pGameEngine->m_PhysicsWorld) {
            ::g_pGameEngine->m_PhysicsWorld->AddBody(rigidBody);
        }
    }
    {
        gdp2022Physics::iRigidBody* rigidBody = 0;
        gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(0, glm::vec3(0.f, -1.f, 0.f));
        gdp2022Physics::cRigidBodyDesc planeDesc;
        planeDesc.linearDamping = 0.f;
        planeDesc.isStatic = true;
        planeDesc.mass = 0.f;
        planeDesc.rotation = glm::quat(glm::vec3(0.f));
        planeDesc.position = glm::vec3(0.f, 1000.f, 0.f);
        planeDesc.linearVelocity = glm::vec3(0.f);

        rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);
        if (rigidBody != 0 && ::g_pGameEngine->m_PhysicsWorld) {
            ::g_pGameEngine->m_PhysicsWorld->AddBody(rigidBody);
        }
    }
    {
        gdp2022Physics::iRigidBody* rigidBody = 0;
        gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(0, glm::vec3(1.f, 0.f, 0.f));
        gdp2022Physics::cRigidBodyDesc planeDesc;
        planeDesc.linearDamping = 0.f;
        planeDesc.isStatic = true;
        planeDesc.mass = 0.f;
        planeDesc.rotation = glm::quat(glm::vec3(0.f));
        planeDesc.position = glm::vec3(-24.f, 0.f, 0.f);
        planeDesc.linearVelocity = glm::vec3(0.f);

        rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);
        if (rigidBody != 0 && ::g_pGameEngine->m_PhysicsWorld) {
            ::g_pGameEngine->m_PhysicsWorld->AddBody(rigidBody);
        }
    }
    {
        gdp2022Physics::iRigidBody* rigidBody = 0;
        gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(0, glm::vec3(-1.f, 0.f, 0.f));
        gdp2022Physics::cRigidBodyDesc planeDesc;
        planeDesc.linearDamping = 0.f;
        planeDesc.isStatic = true;
        planeDesc.mass = 0.f;
        planeDesc.rotation = glm::quat(glm::vec3(0.f));
        planeDesc.position = glm::vec3(24.f, 0.f, 0.f);
        planeDesc.linearVelocity = glm::vec3(0.f);

        rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);
        if (rigidBody != 0 && ::g_pGameEngine->m_PhysicsWorld) {
            ::g_pGameEngine->m_PhysicsWorld->AddBody(rigidBody);
        }
    }
    {
        gdp2022Physics::iRigidBody* rigidBody = 0;
        gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(0, glm::vec3(0.f, 0.f, 1.f));
        gdp2022Physics::cRigidBodyDesc planeDesc;
        planeDesc.linearDamping = 0.f;
        planeDesc.isStatic = true;
        planeDesc.mass = 0.f;
        planeDesc.rotation = glm::quat(glm::vec3(0.f));
        planeDesc.position = glm::vec3(0.f, 0.f, -50.f);
        planeDesc.linearVelocity = glm::vec3(0.f);

        rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);
        if (rigidBody != 0 && ::g_pGameEngine->m_PhysicsWorld) {
            ::g_pGameEngine->m_PhysicsWorld->AddBody(rigidBody);
        }
    }
    {
        gdp2022Physics::iRigidBody* rigidBody = 0;
        gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(0, glm::vec3(0.f, 0.f, -1.f));
        gdp2022Physics::cRigidBodyDesc planeDesc;
        planeDesc.linearDamping = 0.f;
        planeDesc.isStatic = true;
        planeDesc.mass = 0.f;
        planeDesc.rotation = glm::quat(glm::vec3(0.f));
        planeDesc.position = glm::vec3(0.f, 0.f, 350.f);
        planeDesc.linearVelocity = glm::vec3(0.f);

        rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);
        if (rigidBody != 0 && ::g_pGameEngine->m_PhysicsWorld) {
            ::g_pGameEngine->m_PhysicsWorld->AddBody(rigidBody);
        }
    }
}
void GameplayManager::GameStart() {
    ::g_pGameEngine->audioManager.PlayAudio(SOUND_AMBIENCE);
}

void GameplayManager::GameOver() {
    ::g_pGameEngine->audioManager.PlayAudio(SOUND_CANS_GUY);
}

void GameplayManager::Fire() {
    glm::vec3 start = ::g_pFlyCamera->getEye() + (::g_pFlyCamera->getAtDirection() * glm::length(glm::vec3(1.5f, 5.5f, -5.f)));
    glm::vec3 end = (1000.f * glm::normalize(::g_pFlyCamera->getAtDirection())) + ::g_pFlyCamera->getEye();
    gdp2022Physics::iCollisionBody* rayHit = ::g_pGameEngine->m_PhysicsWorld->RayHit(start, end);
    ::g_pGameEngine->audioManager.PlayAudio(SOUND_GUN);

    if (canList.size() > 0) {
        bool hit = false;
        for (unsigned int index = 0; index < canList.size(); index++) {
            if (rayHit == canList[index]->rigidBody) {
                float dist = glm::distance(::g_pFlyCamera->getEye(), canList[index]->mesh->positionXYZ);
                glm::vec3 point = (dist * glm::normalize(::g_pFlyCamera->getAtDirection())) + ::g_pFlyCamera->getEye();
                float distance = glm::distance(canList[index]->mesh->positionXYZ, point);

                if (distance < 1.f) {
                    ::g_pGameEngine->audioManager.PlayAudio(SOUND_PING);
                    canList[index]->rigidBody->ApplyImpulseAtPoint((canList[index]->mesh->positionXYZ - point) * 20.0f / dist, point);
                    canList[index]->Destroy(3.f);
                    canList.erase(canList.begin() + index);
                    if (canList.size() == 0) {
                        GameOver();
                    }
                    cParticleFactory::Instance()->createParticle(PARTICLE_SMOKE, end, false);
                    //Exit early.
                    return;
                }
            }
        }                   
    }
    cParticleFactory::Instance()->createParticle(PARTICLE_SMOKE, end, false);
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