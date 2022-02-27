#pragma once
#include <Graphics/cVAOManager.h>
#include <Graphics/cShaderManager.h>
#include "TextureMapping/cBasicTextureManager.h"
#include <Graphics/cLightManager.h>
#include "EntityManager.h"
#include "AudioManager.h"
#include "AnimationSystem.h"

class cGameEngine {

public:
	cGameEngine();
	virtual ~cGameEngine();

	bool Initialize();
	void Destroy();

	void Update(float deltaTime);
	void Render();

	void LoadAnimationAssignmentOneScene();
	void LoadAnimationAssignmentTwoScene();

	void SetGameSpeed(float speed);
	void ReverseGameSpeed();

	void CycleActiveSequence(int offset = 1);
	std::string GetActiveSequenceName();

	EntityManager entityManager;
	AudioManager audioManager;

	//TODO: Abstract components into their appropriate managers rather than handling as part of the 'main' loop
	cVAOManager* g_pVAOManager;
	cShaderManager* g_pShaderManager;
	cBasicTextureManager* g_pTextureManager;

	cLightManager* g_pTheLights;

	AnimationSystem animationSystem;

	bool keys[348];
	bool showColourChanges;
	bool isMuted;

private:
	//void LoadScene();

	bool isRunning;
	float gameSpeed = 1.f;
	float objectMovementSpeed = 15.f;

	int activeSequenceIndex;

	void HandlePlayerInput();
};