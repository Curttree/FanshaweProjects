#include "UIManager.h"
#include "globals.h"

UIManager* UIManager::_instance = 0;
UIManager::UIManager() {

}

UIManager* UIManager::Instance() {
    if (_instance == 0) {
        _instance = new UIManager;
    }

    return _instance;
}

void UIManager::Initialize() {
    delayTime = 6.f;
    displayTime = 5.f;

    title = ::g_pGameEngine->entityManager.CreateUIEntity();
    title->position = ::g_pFlyCamera->getEye() + ::g_pFlyCamera->getAtDirection() * 5.f + (glm::vec3(0.f, 3.f, 0.f) * ::g_pFlyCamera->getQOrientation());
    title->scale = glm::vec3(0.f, 0.f, 0.f);
    title->mesh = new cMesh("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply");
    title->mesh->scale = title->scale;
    title->mesh->positionXYZ = title->position;
    title->mesh->textureNames[8] = "mission_title.bmp";
    title->mesh->bDontLight = true;
    title->mesh->bUseDiscardTransparency = true;
    title->mesh->orientationXYZ = glm::vec3(0.0f, atan2(title->position.x - ::g_pFlyCamera->getEye().x, title->position.z - ::g_pFlyCamera->getEye().z), 0.0f);
    title->mesh->orientationXYZ += glm::vec3(0.f, glm::pi<float>(), 0.f);
    title->rotation = glm::quat(title->mesh->orientationXYZ);

    for (unsigned int index = 0; index < 5; index++) {
        cEntity* can = ::g_pGameEngine->entityManager.CreateUIEntity();
        can->position = ::g_pFlyCamera->getEye() + ::g_pFlyCamera->getAtDirection() * 5.f + (-1.61f + glm::vec3((float)index *0.21, 2.8f, 0.f) * ::g_pFlyCamera->getQOrientation());
        can->scale = glm::vec3(0.0f, 0.0f, 0.f);
        can->mesh = new cMesh("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply");
        can->mesh->scale = title->scale;
        can->mesh->positionXYZ = title->position;
        can->mesh->textureNames[8] = "can_icon.bmp";
        can->mesh->bUseDiscardTransparency = true;
        can->mesh->bDontLight = true;
        can->mesh->orientationXYZ = glm::vec3(0.0f, atan2(title->position.x - ::g_pFlyCamera->getEye().x, title->position.z - ::g_pFlyCamera->getEye().z), 0.0f);
        can->mesh->orientationXYZ += glm::vec3(0.f, glm::pi<float>(), 0.f);
        can->rotation = glm::quat(title->mesh->orientationXYZ);
        canIcons.push_back(can);
    }
}

void UIManager::TimeStep(float deltaTime) {
    switch (currentState) {
    case UIState::WAITING:
        delayCounter += deltaTime;
        if (delayCounter >= delayTime) {
            currentState = UIState::MISSION_TITLE;
            title->scale = glm::vec3(2.f, 1.f, 1.f);
            for (cEntity* can : canIcons) {
                can->scale = glm::vec3(0.2f, 0.2f, 1.f);
            }
        }
        break;
    case UIState::MISSION_TITLE: {
        displayCounter += deltaTime;
        if (displayCounter >= displayTime) {
            title->scale = glm::vec3(0.f, 0.f, 0.f);
            currentState = UIState::GAMEPLAY;
        }
        break;
    }    
    case UIState::CANS_GUY:
        delayCounter += deltaTime;
        if (delayCounter >= delayTime) {
            currentState = UIState::MISSION_COMPLETE;
            title->mesh->textureNames[8] = "mission_complete.bmp";
            title->scale = glm::vec3(2.f, 1.f, 1.f);
            displayCounter = 0.f;
        }
        break;
    case UIState::MISSION_COMPLETE: {
        displayCounter += deltaTime;
        if (displayCounter >= displayTime) {
            title->scale = glm::vec3(0.f, 0.f, 0.f);
            for (cEntity* can : canIcons) {
                can->scale = glm::vec3(0.f, 0.f, 0.f);
            }
            currentState = UIState::GAME_OVER;
        }
        break;
    }
    default:
        break;
    }
}

//via iGameEventListener
void UIManager::Notify(GameEventType type, void* data) {
    switch (type) {
    case GameEventType::CAN_SHOT: {
        unsigned int index = dynamic_cast<GameEvent_CanShot*>((GameEvent*)data)->GetIndex();
        if (index < canIcons.size()) {
            canIcons[canIcons.size() - 1 - index]->mesh->textureNames[8] = "can_icon_x.bmp";
        }
        break;
    }
    case GameEventType::MISSION_COMPLETE: {
        unsigned int index = dynamic_cast<GameEvent_MissionComplete*>((GameEvent*)data)->GetIndex();
        if (index == 0 && currentState == UIState::GAMEPLAY) {
            currentState = UIState::CANS_GUY;
            delayCounter = 0.f;
            delayTime = 5.f;
        }
        break;
    }
    default:
        break;
    }
}