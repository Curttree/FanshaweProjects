#include "cGameEngine.h"
#include "globals.h"

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
	entityManager.StartUp();

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
	entityManager.ShutDown();

	delete g_pShaderManager;

	delete g_pVAOManager;

	delete g_pTheLights;

	delete g_pTextureManager;
}

void cGameEngine::Update(float dt)
{
	m_PhysicsWorld->TimeStep(dt);
	entityManager.TimeStep(dt);
	MovePlayerObject();
}

void cGameEngine::LoadPhysicsAssignmentOneScene() {
	// Create floor
	{
		cEntity* floor = entityManager.CreateEntity();

		gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(0, glm::vec3(0.0f, 1.0f, 0.0f));
		gdp2022Physics::cRigidBodyDesc planeDesc;
		planeDesc.damping = 0.f;
		planeDesc.isStatic = true;
		planeDesc.mass = 1.f;
		planeDesc.position = glm::vec3(0.f);
		planeDesc.velocity = glm::vec3(0.f);
		floor->rigidBody = m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);

		m_PhysicsWorld->AddRigidBody(floor->rigidBody); 
	}

	// Create Back Wall
	{
		cEntity* wall = entityManager.CreateEntity();

		gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(-25.f, glm::vec3(0.0f, 0.0f, -1.0f));
		gdp2022Physics::cRigidBodyDesc planeDesc;
		planeDesc.damping = 0.f;
		planeDesc.isStatic = true;
		planeDesc.mass = 1.f;
		planeDesc.position = glm::vec3(0.f);
		planeDesc.velocity = glm::vec3(0.f);
		wall->rigidBody = m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);

		m_PhysicsWorld->AddRigidBody(wall->rigidBody);
	}
	// Create Front Wall
	{
		cEntity* wall = entityManager.CreateEntity();

		gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(-25.f, glm::vec3(0.0f, 0.0f, 1.0f));
		gdp2022Physics::cRigidBodyDesc planeDesc;
		planeDesc.damping = 0.f;
		planeDesc.isStatic = true;
		planeDesc.mass = 1.f;
		planeDesc.position = glm::vec3(0.f);
		planeDesc.velocity = glm::vec3(0.f);
		wall->rigidBody = m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);

		m_PhysicsWorld->AddRigidBody(wall->rigidBody);
	}
	// Create Left Wall
	{
		cEntity* wall = entityManager.CreateEntity();

		gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(-25.f, glm::vec3(-1.0f, 0.0f, 0.0f));
		gdp2022Physics::cRigidBodyDesc planeDesc;
		planeDesc.damping = 0.f;
		planeDesc.isStatic = true;
		planeDesc.mass = 1.f;
		planeDesc.position = glm::vec3(0.f);
		planeDesc.velocity = glm::vec3(0.f);
		wall->rigidBody = m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);

		m_PhysicsWorld->AddRigidBody(wall->rigidBody);
	}
	// Create Right Wall
	{
		cEntity* wall = entityManager.CreateEntity();

		gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(-25.f, glm::vec3(1.0f, 0.0f, 0.0f));
		gdp2022Physics::cRigidBodyDesc planeDesc;
		planeDesc.damping = 0.f;
		planeDesc.isStatic = true;
		planeDesc.mass = 1.f;
		planeDesc.position = glm::vec3(0.f);
		planeDesc.velocity = glm::vec3(0.f);
		wall->rigidBody = m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);

		m_PhysicsWorld->AddRigidBody(wall->rigidBody);
	}

	//Create initial 'Player'
	{
		cEntity* ballOne = entityManager.CreateEntity();

		gdp2022Physics::iShape* ballShape = new gdp2022Physics::SphereShape(1.f);
		gdp2022Physics::cRigidBodyDesc sphereDesc;
		sphereDesc.damping = 0.001f;
		sphereDesc.isStatic = false;
		sphereDesc.mass = 1.f;
		sphereDesc.position = glm::vec3(0.f, 1.f, 0.f);
		sphereDesc.velocity = glm::vec3(0.f);
		sphereDesc.restitution = 0.8f;
		ballOne->rigidBody = m_PhysicsFactory->CreateRigidBody(sphereDesc, ballShape);

		cMesh* ballMesh = new cMesh();
		ballMesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
		ballMesh->bIsWireframe = true;
		ballMesh->bUseObjectDebugColour = true;
		ballMesh->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		ballMesh->bDontLight = true;
		ballMesh->scale = 1.0f;
		ballMesh->positionXYZ = glm::vec3(0.f, 1.f, 0.f);
		// Give this a friendly name
		ballMesh->friendlyName = "Ball One";
		ballOne->mesh = ballMesh;

		m_PhysicsWorld->AddRigidBody(ballOne->rigidBody);
		::g_vec_pMeshes.push_back(ballMesh);

		g_pPlayerEntity = ballOne;
	}	
	
	//Create other spheres
	{
		cEntity* ball = entityManager.CreateEntity();

		gdp2022Physics::iShape* ballShape = new gdp2022Physics::SphereShape(2.f);
		gdp2022Physics::cRigidBodyDesc sphereDesc;
		sphereDesc.damping = 0.001f;
		sphereDesc.isStatic = false;
		sphereDesc.mass = 2.f;
		sphereDesc.position = glm::vec3(10.f, 2.f, 0.f);
		sphereDesc.velocity = glm::vec3(0.f);
		sphereDesc.restitution = 0.8f;
		ball->rigidBody = m_PhysicsFactory->CreateRigidBody(sphereDesc, ballShape);

		cMesh* ballMesh = new cMesh();
		ballMesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
		ballMesh->bIsWireframe = true;
		ballMesh->bUseObjectDebugColour = true;
		ballMesh->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		ballMesh->bDontLight = true;
		ballMesh->scale = 2.0f;
		ballMesh->positionXYZ = glm::vec3(10.f, 2.f, 0.f);
		// Give this a friendly name
		ballMesh->friendlyName = "Ball Two";
		ball->mesh = ballMesh;

		m_PhysicsWorld->AddRigidBody(ball->rigidBody);
		::g_vec_pMeshes.push_back(ballMesh);
	}

	{
		cEntity* ball = entityManager.CreateEntity();

		gdp2022Physics::iShape* ballShape = new gdp2022Physics::SphereShape(0.5f);
		gdp2022Physics::cRigidBodyDesc sphereDesc;
		sphereDesc.damping = 0.001f;
		sphereDesc.isStatic = false;
		sphereDesc.mass = 0.5f;
		sphereDesc.position = glm::vec3(-10.f, 0.5f, 10.f);
		sphereDesc.velocity = glm::vec3(0.f);
		sphereDesc.restitution = 0.8f;
		ball->rigidBody = m_PhysicsFactory->CreateRigidBody(sphereDesc, ballShape);

		cMesh* ballMesh = new cMesh();
		ballMesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
		ballMesh->bIsWireframe = true;
		ballMesh->bUseObjectDebugColour = true;
		ballMesh->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		ballMesh->bDontLight = true;
		ballMesh->scale = 0.5f;
		ballMesh->positionXYZ = glm::vec3(-10.f, 0.5f, 10.f);
		// Give this a friendly name
		ballMesh->friendlyName = "Ball Three";
		ball->mesh = ballMesh;

		m_PhysicsWorld->AddRigidBody(ball->rigidBody);
		::g_vec_pMeshes.push_back(ballMesh);
	}
	{
		cEntity* ball = entityManager.CreateEntity();

		gdp2022Physics::iShape* ballShape = new gdp2022Physics::SphereShape(1.25f);
		gdp2022Physics::cRigidBodyDesc sphereDesc;
		sphereDesc.damping = 0.001f;
		sphereDesc.isStatic = false;
		sphereDesc.mass = 1.25f;
		sphereDesc.position = glm::vec3(0.f, 1.25f, 10.f);
		sphereDesc.velocity = glm::vec3(0.f);
		sphereDesc.restitution = 0.8f;
		ball->rigidBody = m_PhysicsFactory->CreateRigidBody(sphereDesc, ballShape);

		cMesh* ballMesh = new cMesh();
		ballMesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
		ballMesh->bIsWireframe = true;
		ballMesh->bUseObjectDebugColour = true;
		ballMesh->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		ballMesh->bDontLight = true;
		ballMesh->scale = 1.25f;
		ballMesh->positionXYZ = glm::vec3(0.f, 1.25f, 10.f);
		// Give this a friendly name
		ballMesh->friendlyName = "Ball Three";
		ball->mesh = ballMesh;

		m_PhysicsWorld->AddRigidBody(ball->rigidBody);
		::g_vec_pMeshes.push_back(ballMesh);
	}
	{
		cEntity* ball = entityManager.CreateEntity();

		gdp2022Physics::iShape* ballShape = new gdp2022Physics::SphereShape(0.75f);
		gdp2022Physics::cRigidBodyDesc sphereDesc;
		sphereDesc.damping = 0.001f;
		sphereDesc.isStatic = false;
		sphereDesc.mass = 0.75f;
		sphereDesc.position = glm::vec3(0.f, 0.75f, -10.f);
		sphereDesc.velocity = glm::vec3(0.f);
		sphereDesc.restitution = 0.8f;
		ball->rigidBody = m_PhysicsFactory->CreateRigidBody(sphereDesc, ballShape);

		cMesh* ballMesh = new cMesh();
		ballMesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
		ballMesh->bIsWireframe = true;
		ballMesh->bUseObjectDebugColour = true;
		ballMesh->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		ballMesh->bDontLight = true;
		ballMesh->scale = 0.75f;
		ballMesh->positionXYZ = glm::vec3(0.f, 0.75f, -10.f);
		// Give this a friendly name
		ballMesh->friendlyName = "Ball Three";
		ball->mesh = ballMesh;

		m_PhysicsWorld->AddRigidBody(ball->rigidBody);
		::g_vec_pMeshes.push_back(ballMesh);
	}
}

void cGameEngine::MovePlayerObject() {
	if (keys[GLFW_KEY_S])
	{
		::g_pPlayerEntity->rigidBody->ApplyForce(glm::normalize(::g_pFlyCamera->getAt()) * objectMovementSpeed);
	}
	if (keys[GLFW_KEY_W])
	{
		::g_pPlayerEntity->rigidBody->ApplyForce(glm::normalize(::g_pFlyCamera->getAt()) * -objectMovementSpeed);
	}
	if (keys[GLFW_KEY_A])
	{
		::g_pPlayerEntity->rigidBody->ApplyForce(glm::normalize(::g_pFlyCamera->getLeft()) * -objectMovementSpeed);
	}
	if (keys[GLFW_KEY_D])
	{
		::g_pPlayerEntity->rigidBody->ApplyForce(glm::normalize(::g_pFlyCamera->getLeft()) * objectMovementSpeed);
	}
}