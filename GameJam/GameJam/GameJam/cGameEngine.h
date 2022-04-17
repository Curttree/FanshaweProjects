#pragma once
#include <Graphics/cVAOManager.h>
#include <Graphics/cShaderManager.h>
#include "TextureMapping/cBasicTextureManager.h"
#include <Graphics/cLightManager.h>
#include "EntityManager.h"
#include <Animation/AudioManager.h>
#include <Animation/AnimationSystem.h>
#include <Animation/BoneSystem.h>
#include "GameplayManager.h"
#include "../PhysicsInterface/Fanshawe2022Physics/interfaces/iPhysicsFactory.h"

class cGameEngine {

public:
	cGameEngine();
	virtual ~cGameEngine();

	bool Initialize();
	void Destroy();

	void Update(float deltaTime);
	void Render();

	void SetGameSpeed(float speed);
	void ReverseGameSpeed();

	EntityManager entityManager;
	AudioManager audioManager;

	//TODO: Abstract components into their appropriate managers rather than handling as part of the 'main' loop
	cVAOManager* g_pVAOManager;
	cShaderManager* g_pShaderManager;
	cBasicTextureManager* g_pTextureManager;

	cLightManager* g_pTheLights;

	AnimationSystem animationSystem;
	BoneSystem boneSystem;

	GameplayManager* g_pGameplayManager;

	bool keys[348];
	bool showColourChanges;
	bool isMuted;

	//TODO: Make private but have getters.
	gdp2022Physics::iPhysicsFactory* m_PhysicsFactory;
	gdp2022Physics::iPhysicsWorld* m_PhysicsWorld;

private:
	//void LoadScene();

	bool isRunning;
	float gameSpeed = 1.f;
	float objectMovementSpeed = 15.f;

	int activeSequenceIndex;

	void HandlePlayerInput();
};