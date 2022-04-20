#include "cCharacter.h"
#include <GLFW/glfw3.h>
#include <Character.h>

cCharacter::cCharacter(glm::vec3 startPosition, glm::vec3 startOrientation) {
    cMesh* character_mesh = new cMesh();
    character_mesh->setAllMeshNames("debugMan.ply");
    character_mesh->scale = glm::vec3(0.01f);
    character_mesh->positionXYZ = startPosition;
    character_mesh->orientationXYZ = startOrientation;
    character_mesh->textureNames[0] = "cue.bmp";
    character_mesh->textureRatios[0] = 1.f;
    mesh = character_mesh;
    this->position = character_mesh->positionXYZ;
    this->rotation = glm::quat(character_mesh->orientationXYZ);
    this->scale = character_mesh->scale;
    BuildAnimationTransitions();
    physicsCharacter = new gdp2022Physics::Character();
    physicsCharacter->Initialize();
}

//via cEntity
void cCharacter::TimeStep(float deltaTime) {
    animationStateMachine.Update(deltaTime);
    if (!isIdle) {
        idleTimer += deltaTime;
        if (idleTimer >= maxIdleTime) {
            isIdle = true;        
            GameEvent_AFK* g_event = new GameEvent_AFK();
            animationStateMachine.Notify(GameEventType::AFK, g_event);
            delete g_event;
        }
    }
}

std::string  cCharacter::GetAnimationStateAsString(void) {
    return states[animationStateMachine.GetCurrentState()];
}

AnimationState cCharacter::GetAnimationState(void) {
    return animationStateMachine.GetCurrentState();
}

//via iGameEventListener
void cCharacter::Notify(GameEventType type, void* data) {
    // We have a notification! Reset the idle timer and wake up if they are idle.
    idleTimer = 0.f;
    animationStateMachine.Notify(type, data);
    if (isIdle) {
        isIdle = false;
        GameEvent_BackInAction* g_event = new GameEvent_BackInAction();
        animationStateMachine.Notify(GameEventType::BACK_IN_ACTION, g_event);
        delete g_event;
    }
    //Also let the animation state machine know about this event.
}

void cCharacter::BuildAnimationTransitions(void) {
    //Walking states
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W), AnimationState::Idle, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A), AnimationState::Idle, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S), AnimationState::Idle, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D), AnimationState::Idle, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W), AnimationState::Waiting, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A), AnimationState::Waiting, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S), AnimationState::Waiting, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D), AnimationState::Waiting, AnimationState::Walk, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W), AnimationState::Run, AnimationState::Walk, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A), AnimationState::Run, AnimationState::Walk, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S), AnimationState::Run, AnimationState::Walk, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D), AnimationState::Run, AnimationState::Walk, 1.0f);

    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_LEFT_SHIFT), AnimationState::Run, AnimationState::Walk, 1.0f);

    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_W), AnimationState::Walk, AnimationState::Idle, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_A), AnimationState::Walk, AnimationState::Idle, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_S), AnimationState::Walk, AnimationState::Idle, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_D), AnimationState::Walk, AnimationState::Idle, 0.5f);

    //Running states
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W,true), AnimationState::Idle, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A, true), AnimationState::Idle, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S, true), AnimationState::Idle, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D, true), AnimationState::Idle, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W, true), AnimationState::Waiting, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A, true), AnimationState::Waiting, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S, true), AnimationState::Waiting, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D, true), AnimationState::Waiting, AnimationState::Run, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_W, true), AnimationState::Walk, AnimationState::Run, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_A, true), AnimationState::Walk, AnimationState::Run, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_S, true), AnimationState::Walk, AnimationState::Run, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_D, true), AnimationState::Walk, AnimationState::Run, 1.0f);

    //Not sure what order this will be sent in. Can test and eliminate redundant transition.
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_LEFT_SHIFT), AnimationState::Walk, AnimationState::Run, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_LEFT_SHIFT, true), AnimationState::Walk, AnimationState::Run, 1.0f);

    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_W), AnimationState::Run, AnimationState::Idle, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_A), AnimationState::Run, AnimationState::Idle, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_S), AnimationState::Run, AnimationState::Idle, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_KeyRelease(GLFW_KEY_D), AnimationState::Run, AnimationState::Idle, 1.0f);

    //Shooting
    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Idle, AnimationState::Aim, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Waiting, AnimationState::Aim, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Walk, AnimationState::Aim, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Run, AnimationState::Aim, 0.5f);

    animationStateMachine.AddTransition(new GameEvent_MousePress(GLFW_MOUSE_BUTTON_LEFT), AnimationState::Aim, AnimationState::Shoot, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_AnimationExit(), AnimationState::Shoot, AnimationState::Aim, 1.0f);

    animationStateMachine.AddTransition(new GameEvent_MouseRelease(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Aim, AnimationState::Idle, 1.0f);
    animationStateMachine.AddTransition(new GameEvent_MouseRelease(GLFW_MOUSE_BUTTON_RIGHT), AnimationState::Shoot, AnimationState::Idle, 1.0f);

    //Actions
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_Q), AnimationState::Idle, AnimationState::Waiting, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_KeyPress(GLFW_KEY_Q,true), AnimationState::Idle, AnimationState::Waiting, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_AFK, AnimationState::Idle, AnimationState::Waiting, 0.5f);

    animationStateMachine.AddTransition(new GameEvent_BackInAction(), AnimationState::Waiting, AnimationState::Idle, 0.5f);
    animationStateMachine.AddTransition(new GameEvent_AnimationExit(), AnimationState::Waiting, AnimationState::Idle, 0.5f);

    //Temp - Build out animation state string representations for debugging.
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Idle, "Idle"));
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Waiting, "Waiting"));
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Walk, "Walk"));
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Run, "Run"));
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Aim, "Aim"));
    states.insert(std::pair< AnimationState, std::string>(AnimationState::Shoot, "Shoot"));
}