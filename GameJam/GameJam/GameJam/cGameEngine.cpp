#include "cGameEngine.h"
#include "globals.h"

#include <Animation/cBone.h>

// If I am going to continue to add commands, I should switch to a factory.
#include <Animation/cCommand_ConsoleOutput.h>
#include <Animation/cCommand_PlaySound.h>

cGameEngine::cGameEngine(void)
{
}

cGameEngine::~cGameEngine(void)
{
}

bool cGameEngine::Initialize(void)
{
	// init managers
	entityManager.StartUp();
	audioManager.StartUp();

	g_pShaderManager = new cShaderManager();

	g_pTheLights = new cLightManager();

	g_pVAOManager = new cVAOManager();

	g_pTextureManager = new cBasicTextureManager();

	return true;
}

void cGameEngine::Destroy(void)
{
	entityManager.ShutDown();
	audioManager.ShutDown();

	delete g_pShaderManager;

	delete g_pVAOManager;

	delete g_pTheLights;

	delete g_pTextureManager;
}

void cGameEngine::Update(float dt)
{
	//animationSystem.Process(entityManager.GetEntities(), dt * gameSpeed);
	boneSystem.Process(entityManager.GetEntities(), dt * gameSpeed);
	entityManager.TimeStep(dt);
	HandlePlayerInput();
}

void cGameEngine::SetGameSpeed(float speed) {
	// Update game speed only for the active sequence. Uncomment code for the entire game speed.
	//gameSpeed /= glm::abs(gameSpeed);
	//gameSpeed *= speed;
	entityManager.GetEntities()[activeSequenceIndex]->animations.speed = speed;
}
void cGameEngine::ReverseGameSpeed() {
	gameSpeed *= -1.f;
}

glm::vec3 GetEasingColour(EasingType type) {
	switch (type)
	{
	case EasingType::EaseIn:
		// Red
		return glm::vec3(1.f, 0.f, 0.f);
		break;
	case EasingType::EaseOut:
		// Yellow
		return glm::vec3(1.f, 1.f, 0.f);
		break;
	case EasingType::EaseInOut:
		// Green
		return glm::vec3(0.f, 1.f, 0.f);
		break;
	default:
		// No easing...White.
		return glm::vec3(1.f);
		break;
	}
}

void cGameEngine::HandlePlayerInput() {
	if (keys[GLFW_KEY_1]) {
		boneSystem.PlaySequence(entityManager.GetEntities(), 0);
		keys[GLFW_KEY_1] = false;
	}
	else if (keys[GLFW_KEY_2]) {
		boneSystem.PlaySequence(entityManager.GetEntities(), 1);
		keys[GLFW_KEY_2] = false;
	}
	else if (keys[GLFW_KEY_3]) {
		boneSystem.PlaySequence(entityManager.GetEntities(), 2);
		keys[GLFW_KEY_3] = false;
	}
	else if (keys[GLFW_KEY_4]) {
		boneSystem.PlaySequence(entityManager.GetEntities(), 3);
		keys[GLFW_KEY_4] = false;
	}
	else if (keys[GLFW_KEY_5]) {
		boneSystem.PlaySequence(entityManager.GetEntities(), 4);
		keys[GLFW_KEY_5] = false;
	}
	else if (keys[GLFW_KEY_KP_1]) {
		boneSystem.PlaySequence(entityManager.GetEntities(), 0);
		keys[GLFW_KEY_KP_1] = false;
	}
	else if (keys[GLFW_KEY_KP_2]) {
		boneSystem.PlaySequence(entityManager.GetEntities(), 1);
		keys[GLFW_KEY_KP_2] = false;
	}
	else if (keys[GLFW_KEY_KP_3]) {
		boneSystem.PlaySequence(entityManager.GetEntities(), 2);
		keys[GLFW_KEY_KP_3] = false;
	}
	else if (keys[GLFW_KEY_KP_4]) {
		boneSystem.PlaySequence(entityManager.GetEntities(), 3);
		keys[GLFW_KEY_KP_4] = false;
	}
	else if (keys[GLFW_KEY_KP_5]) {
		boneSystem.PlaySequence(entityManager.GetEntities(), 4);
		keys[GLFW_KEY_KP_5] = false;
	}
}