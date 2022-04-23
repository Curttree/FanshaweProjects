#include "cCharacter.h"
#include <Character.h>
#include "GameEvents.h"
#include "globals.h"
#include <iostream>
#include <glm/gtx/matrix_decompose.hpp>
#include <PhysicsConversion.h>
#include <../BulletCollision/CollisionDispatch/btGhostObject.h>

cCharacter::cCharacter(glm::vec3 startPosition, glm::vec3 startOrientation) {
    //Mesh
    cMesh* character_mesh = new cMesh("Characters/Detective/detective@aim.fbx");
    character_mesh->scale = glm::vec3(0.00025f);
    character_mesh->positionXYZ = startPosition;
    character_mesh->orientationXYZ = startOrientation;
    character_mesh->textureNames[0] = "Fish_BaseColor.bmp";
    character_mesh->textureRatios[0] = 1.f;
    character_mesh->bUseSpecularMap = true;
    character_mesh->specularMapTexture = "Male_16_Specular.bmp";
    character_mesh->wholeObjectShininess_SpecPower = 0.01f;
    mesh = character_mesh;
    this->position = character_mesh->positionXYZ;
    this->rotation = glm::quat(character_mesh->orientationXYZ);
    this->scale = character_mesh->scale;
    proxyOffset = glm::vec3(0.f, 2.5f, 0.f);

    //Child Mesh
    cMesh* gun = new cMesh("ColtPython.ply");
    gun->scale = glm::vec3(4000.f);
    gun->positionXYZ = glm::vec3(-500.f,13200.f,6200.f);
    gun->textureNames[0] = "BrushedMetalSilver2.bmp";
    gun->textureRatios[0] = 1.f;
    gun->wholeObjectShininess_SpecPower = 0.0001f;
    mesh->vec_pChildMeshes.push_back(gun);


    //Animations
    BuildAnimationTransitions(); 
    BuildAnimationMap();

    //Physics
    btVector3 up;
    gdp2022Physics::CastBulletVector3(glm::vec3(0.f, 1.f, 0.f), &up);

    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(0, 0, 0));

    //TODO: Eventually switch to btKinematicCharacter controller. Move this to other project to remove direct bullet dependency.
    //Ghost object code adapted from https://github.com/kripken/bullet/blob/master/Demos/CharacterDemo/CharacterDemo.cpp
    //btVector3 worldMin(-1000, -1000, -1000);
    //btVector3 worldMax(1000, 1000, 1000);
    //btVector3 charMin(-1, -1, -1);
    //btVector3 charMax(1, 1, 1);
    //btPairCachingGhostObject* m_ghostObject = new btPairCachingGhostObject();
    //m_ghostObject->setWorldTransform(startTransform); 
    //btAxisSweep3* sweepBP = new btAxisSweep3(worldMin, worldMax);
    //sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    //btScalar characterHeight = 1.75;
    //btScalar characterWidth = 1.75;
    //btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
    //m_ghostObject->setCollisionShape(capsule);
    //m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

    //btBroadphaseProxy* broadphase = new btBroadphaseProxy(charMin, charMax, m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::DefaultFilter);
    //m_ghostObject->setBroadphaseHandle(broadphase);
    //broadphase->m_clientObject = m_ghostObject;
    //physicsCharacter = new gdp2022Physics::Character((btCollisionWorld*)::g_pGameEngine->m_PhysicsWorld->getDynamicsWorld(),m_ghostObject, up);
    //physicsCharacter->Initialize();

    physicsProxy = new cPlayerProxy(this->position+proxyOffset);
}
bool cCharacter::GetGunPosition(glm::vec3& position) {
    if (animationStateMachine.GetTransitioning() && animationStateMachine.GetTransitioningState() == AnimationState::Idle) {
        return false;
    }
    switch (animationStateMachine.GetCurrentState()) {
    case AnimationState::Aim:
        position = glm::vec3(-500.f, 13200.f, 6200.f);
        return true;
        break;
    case AnimationState::Shoot:
        position = glm::vec3(-1400.f, 12000.f, 7400.f);
        return true;
        break;
    default:
        return false;
    }
}

void cCharacter::LoadBones() {
    ::g_pVAOManager->FindBonesByModelName("Characters/Detective/detective@aim.fbx", mesh->bones);
    if (mesh->bones->bones.size()>0) {
        mesh->bUseBones = true;
    }
}

void cCharacter::LoadAnimation() {
    ::g_pVAOManager->FindAnimationByName(GetAnimationFromState(animationStateMachine.GetCurrentState()), this->current_animation);
    if (this->current_animation) {
        animation = *current_animation;
        animation.shouldPlay = true;
        animation.speed = animationStateMachine.GetCurrentState() == AnimationState::Shoot ? 2.f : 1.f;
    }
    // We are loading a new animation. Check to see if the gun should be drawn alongside the animation.
    glm::vec3 gunPos=glm::vec3(0.f);
    if (GetGunPosition(gunPos)) {
        mesh->vec_pChildMeshes[0]->positionXYZ = gunPos;
        mesh->vec_pChildMeshes[0]->scale = glm::vec3(4000.f);
    }
    else {
        mesh->vec_pChildMeshes[0]->scale = glm::vec3(0.f);
    }

}

bool cCharacter::isRunning() {
    return animationStateMachine.GetCurrentState() == AnimationState::Run;
}


//via cEntity
void cCharacter::TimeStep(float deltaTime) {
    //physicsCharacter->TimeStep(deltaTime);
    physicsProxy->TimeStep(deltaTime);
    if (!::g_pGameEngine->g_pGameplayManager->GetAiming()) {
        position = physicsProxy->position - proxyOffset;
        rotation = physicsProxy->rotation;
    }
    animationStateMachine.Update(deltaTime);
    
    UpdateAnimationBlend();
    if (!isIdle) {
        idleTimer += deltaTime;
        if (idleTimer >= maxIdleTime) {
            isIdle = true;        
            GameEvent_AFK* g_event = new GameEvent_AFK();
            animationStateMachine.Notify(GameEventType::AFK, g_event);
            delete g_event;
        }
    }
    if (isStopping) {
        stoppingTimer += deltaTime;
        if (stoppingTimer >= maxStoppingTime && !checkMovementKeysDown()) {
            //We have given them enough time to change input. Send our notification to the animation state controller.
            //Since our animations are the same for any direction, we are just going to simulate releasing the forward key. If the behavior for different directions changes later, track which key was last pressed.
            GameEvent_KeyRelease* g_event = new GameEvent_KeyRelease(GLFW_KEY_W);
            animationStateMachine.Notify(GameEventType::KEY_RELEASE, g_event);
            isStopping = false;
            stoppingTimer = 0.f;
        }
    }
    if (animationStateMachine.GetCurrentState() == AnimationState::Idle && glm::length(physicsProxy->velocity) > 0.01f) {
        // Uh oh, a movement input was missed. Do cleanup.
        if (cGFLWKeyboardModifiers::isModifierDown(::g_pWindow, true, false, false)) {
            GameEvent_KeyPress* g_event = new GameEvent_KeyPress(GLFW_KEY_W, true);
            animationStateMachine.Notify(GameEventType::KEY_PRESS, g_event);
        }
        else {
            GameEvent_KeyPress* g_event = new GameEvent_KeyPress(GLFW_KEY_W, false);
            animationStateMachine.Notify(GameEventType::KEY_PRESS, g_event);
        }
    }
    glm::vec3 pos;
    if (!GetGunPosition(pos)) {
        mesh->vec_pChildMeshes[0]->scale = glm::vec3(0.f);
    }
    cEntity::TimeStep(deltaTime);

    //glm::vec3 glmScale;
    //glm::quat glmOrientation;
    //glm::vec3 glmTranslation;
    //glm::vec3 glmSkew;
    //glm::vec4 glmPerspective;
    //glm::vec3 positionQuestionMark = glm::vec4(0.f);
    //positionQuestionMark += mesh->meshData->mBoneData[rightHandIndex].FinalTransformation * glm::vec4(position,1.f) * vBoneWeights[0];
    //positionQuestionMark += BoneMatrices[int(vBoneIDs[1])] * vPosition * vBoneWeights[1];
    //positionQuestionMark += BoneMatrices[int(vBoneIDs[2])] * vPosition * vBoneWeights[2];
    //positionQuestionMark += BoneMatrices[int(vBoneIDs[3])] * vPosition * vBoneWeights[3];
    //glm::decompose(mesh->meshData->mBoneData[rightHandIndex].GlobalTransformation, glmScale, glmOrientation, glmTranslation, glmSkew, glmPerspective);
    //std::cout << glmTranslation.x << " " << glmTranslation.y <<" "<< glmTranslation.z <<  std::endl;
}

void cCharacter::UpdateAnimationBlend() {
    Transition currentTransition;
    float factor;
    if (animationStateMachine.GetTransitioning(currentTransition, factor)) {
        ::g_pVAOManager->FindAnimationByName(GetAnimationFromState(currentTransition.from), animationBlend.anim1);
        ::g_pVAOManager->FindAnimationByName(GetAnimationFromState(currentTransition.to), animationBlend.anim2);
        animationBlend.direction = 1.f;
        animationBlend.speed = 1.f;
        animationBlend.factor = factor;
    }
    else {
        animationBlend.anim1 = nullptr;
        animationBlend.anim2 = nullptr;
        animationBlend.direction = 1.f;
        animationBlend.speed = 1.f;
        animationBlend.factor = 0.f;
    }
}

std::string  cCharacter::GetAnimationStateAsString(void) {
    return states[animationStateMachine.GetCurrentState()];
}

AnimationState cCharacter::GetAnimationState(void) {
    return animationStateMachine.GetCurrentState();
}

AnimationState cCharacter::GetFutureAnimationState(void) {
    return animationStateMachine.GetTransitioningState();
}

std::string cCharacter::GetAnimationFromState(AnimationState state) {
    std::map< AnimationState, std::string>::iterator anim = map_animations.find(state);
    return  anim != map_animations.end() ? anim->second : "Characters/Detective/detective@idle.fbx";
}

//via iGameEventListener
void cCharacter::Notify(GameEventType type, void* data) {
    // We have a notification! Reset the idle timer and wake up if they are idle.
    idleTimer = 0.f;
    if (!isStopping && type == GameEventType::KEY_RELEASE) {
        unsigned char key = dynamic_cast<GameEvent_KeyRelease*>((GameEvent*)data)->GetKey();
        if (key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D) {
            //Don't notify the animation state machine yet. Buffer the input in case the user is just changing directions.
            isStopping = true;
        }
        else {
            animationStateMachine.Notify(type, data);
        }
    }
    else if (isStopping && type == GameEventType::KEY_PRESS) {
        //They might be moving again. Verify.
        unsigned char key = dynamic_cast<GameEvent_KeyPress*>((GameEvent*)data)->GetKey();
        if ((key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D)&& animationStateMachine.GetCurrentState() != AnimationState::Idle) {
            //They are still moving. Reset the stop count and swallow the animation state change.
            stoppingTimer = 0.f;
            isStopping = false;
        }
        else {
            //There could be another relevant key press. Send to the animation state machine.
            animationStateMachine.Notify(type, data);
        }
    }
    else {
        animationStateMachine.Notify(type, data);
    }
    if (isIdle) {
        isIdle = false;
        GameEvent_BackInAction* g_event = new GameEvent_BackInAction();
        animationStateMachine.Notify(GameEventType::BACK_IN_ACTION, g_event);
        delete g_event;
    }
    //Also let the animation state machine know about this event.
}

void cCharacter::SetNeutralOrientation() {
    neutralOrientation = rotation;
}

void cCharacter::ResetToNeutralOrientation() {
    rotation = neutralOrientation;
}

void cCharacter::BuildAnimationMap(void) {
    map_animations.insert(std::make_pair(AnimationState::Idle, "Characters/Detective/detective@idle.fbx"));
    map_animations.insert(std::make_pair(AnimationState::Aim, "Characters/Detective/detective@aim.fbx"));
    map_animations.insert(std::make_pair(AnimationState::Shoot, "Characters/Detective/detective@shoot.fbx"));
    map_animations.insert(std::make_pair(AnimationState::Waiting, "Characters/Detective/detective@wait.fbx"));
    map_animations.insert(std::make_pair(AnimationState::Walk, "Characters/Detective/detective@walk.fbx"));
    map_animations.insert(std::make_pair(AnimationState::Run, "Characters/Detective/detective@run.fbx"));
}

void cCharacter::BuildAnimationTransitions(void) {
    //Need to get the animation for shooting & waiting so we can use their exit as valid transitions.
    Animation* shootAnim;
    Animation* waitAnim;

    //Walking states
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W), AnimationState::Idle, AnimationState::Walk, 0.1f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A), AnimationState::Idle, AnimationState::Walk, 0.1f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S), AnimationState::Idle, AnimationState::Walk, 0.1f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D), AnimationState::Idle, AnimationState::Walk, 0.1f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W), AnimationState::Waiting, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A), AnimationState::Waiting, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S), AnimationState::Waiting, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D), AnimationState::Waiting, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W), AnimationState::Run, AnimationState::Walk, 0.25f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A), AnimationState::Run, AnimationState::Walk, 0.25f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S), AnimationState::Run, AnimationState::Walk, 0.25f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D), AnimationState::Run, AnimationState::Walk, 0.25f);

    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_LEFT_SHIFT), AnimationState::Run, AnimationState::Walk, 0.2f);

    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_W), AnimationState::Walk, AnimationState::Idle, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_A), AnimationState::Walk, AnimationState::Idle, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_S), AnimationState::Walk, AnimationState::Idle, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_D), AnimationState::Walk, AnimationState::Idle, 0.5f);

    //Running states
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W,true), AnimationState::Idle, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A, true), AnimationState::Idle, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S, true), AnimationState::Idle, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D, true), AnimationState::Idle, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W, true), AnimationState::Waiting, AnimationState::Run, 0.75f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A, true), AnimationState::Waiting, AnimationState::Run, 0.75f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S, true), AnimationState::Waiting, AnimationState::Run, 0.75f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D, true), AnimationState::Waiting, AnimationState::Run, 0.75f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W, true), AnimationState::Walk, AnimationState::Run, 0.25f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A, true), AnimationState::Walk, AnimationState::Run, 0.25f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S, true), AnimationState::Walk, AnimationState::Run, 0.25f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D, true), AnimationState::Walk, AnimationState::Run, 0.25f);

    //Not sure what order this will be sent in. Can test and eliminate redundant transition.
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_LEFT_SHIFT), AnimationState::Walk, AnimationState::Run, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_LEFT_SHIFT, true), AnimationState::Walk, AnimationState::Run, 1.0f);

    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_W), AnimationState::Run, AnimationState::Idle, 0.1f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_A), AnimationState::Run, AnimationState::Idle, 0.1f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_S), AnimationState::Run, AnimationState::Idle, 0.1f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_D), AnimationState::Run, AnimationState::Idle, 0.1f);

    //Shooting
    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Idle, AnimationState::Aim, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Waiting, AnimationState::Aim, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Walk, AnimationState::Aim, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Run, AnimationState::Aim, 0.5f);

    ::g_pVAOManager->FindAnimationByName(GetAnimationFromState(AnimationState::Shoot), shootAnim);
    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_LEFT), AnimationState::Aim, AnimationState::Shoot, 0.25f);
    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_LEFT), AnimationState::Shoot, AnimationState::Shoot, 0.1f);
    animationStateMachine.AddTransition(new GameEvent_AnimationExit(static_cast<unsigned int>(AnimationState::Shoot)), AnimationState::Shoot, AnimationState::Aim, 0.5f);

    animationStateMachine.AddTransition(new GameEvent_MouseRelease(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Aim, AnimationState::Idle, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_MouseRelease(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Shoot, AnimationState::Idle, 1.0f);

    //Actions
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_Q), AnimationState::Idle, AnimationState::Waiting, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_Q,true), AnimationState::Idle, AnimationState::Waiting, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_AFK, AnimationState::Idle, AnimationState::Waiting, 0.5f);

    ::g_pVAOManager->FindAnimationByName(GetAnimationFromState(AnimationState::Waiting), waitAnim);
    animationStateMachine.AddTransition(new GameEvent_BackInAction(), AnimationState::Waiting, AnimationState::Idle, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_AnimationExit(static_cast<unsigned int>(AnimationState::Waiting)), AnimationState::Waiting, AnimationState::Idle, 0.5f);

    //Temp - Build out animation state string representations for debugging.
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Idle, "Idle"));
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Waiting, "Waiting"));
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Walk, "Walk"));
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Run, "Run"));
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Aim, "Aim"));
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Shoot, "Shoot"));
}