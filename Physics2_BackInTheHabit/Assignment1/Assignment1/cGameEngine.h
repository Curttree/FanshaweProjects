#pragma once
#include <Graphics/cVAOManager.h>
#include <Graphics/cShaderManager.h>
#include "TextureMapping/cBasicTextureManager.h"
#include <Graphics/cLightManager.h>
// Physics
#include <Fanshawe2022Physics/interfaces/iPhysicsFactory.h>
#include <Fanshawe2022Physics/interfaces/iPhysicsWorld.h>
#include <Fanshawe2022Physics/interfaces/iCollisionListener.h>
class cGameEngine {

public:
	cGameEngine();
	virtual ~cGameEngine();

	bool Initialize();
	void Destroy();

	//void Update(float deltaTime);
	//void Render();

	//TODO: Abstract components into their appropriate managers rather than handling as part of the 'main' loop
	cVAOManager* g_pVAOManager;
	cShaderManager* g_pShaderManager;
	cBasicTextureManager* g_pTextureManager;

	cLightManager* g_pTheLights;


private:
	//void LoadScene();

	bool isRunning;
	bool isPaused;
	float gameSpeed = 1.f;

	gdp2022Physics::iPhysicsFactory* m_PhysicsFactory;
	gdp2022Physics::iPhysicsWorld* m_PhysicsWorld;

};