#include "cGameEngine.h"

#include <Fanshawe2022Physics/PhysicsFactory.h>
#include "CollisionListener.h"
#include <Fanshawe2022Physics/interfaces/shapes.h>
#include <Fanshawe2022Physics/interfaces/cRigidBodyDesc.h>

cGameEngine::cGameEngine(void)
{
}

cGameEngine::~cGameEngine(void)
{
}

bool cGameEngine::Initialize(void)
{
	// init managers

	g_pShaderManager = new cShaderManager();

	g_pTheLights = new cLightManager();

	g_pVAOManager = new cVAOManager();

	g_pTextureManager = new cBasicTextureManager();

	// Physics Initialization, Useing our Factory to create
	m_PhysicsFactory = new gdp2022Physics::PhysicsFactory();
	m_PhysicsWorld = m_PhysicsFactory->CreateWorld();
	m_PhysicsWorld->RegisterCollisionListener(new gdp2022Physics::CollisionListener());
	m_PhysicsWorld->SetGravity(glm::vec3(0.f, -9.81f, 0.f));

	//LoadDemoScene(PHYSICS_SCENE_INDEX);

	/*for (int i = 0; i < 255; ++i)
		m_KeyDown[i] = false;*/

	return true;
}

void cGameEngine::Destroy(void)
{
	delete g_pShaderManager;

	delete g_pVAOManager;

	delete g_pTheLights;

	delete g_pTextureManager;
}