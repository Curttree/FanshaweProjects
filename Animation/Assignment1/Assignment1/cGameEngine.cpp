#include "cGameEngine.h"
#include "globals.h"

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
	if (!isPaused) {
		animationSystem.Process(entityManager.GetEntities(), dt * gameSpeed);
	}
	entityManager.TimeStep(dt);
	MovePlayerObject();
}

void cGameEngine::SetGameSpeed(float speed) {
	gameSpeed = speed;
}
void cGameEngine::ReverseGameSpeed() {
	gameSpeed *= -1.f;
}

void cGameEngine::MovePlayerObject() {

}

void cGameEngine::LoadAnimationAssignmentOneScene() {
	//Skeleton 
	cEntity* skel = entityManager.CreateEntity();

	skel->animation.currentTime = 0;
	skel->animation.duration = 6.0f;
	skel->animation.playing = true;
	skel->animation.speed = 1.f;
	skel->animation.repeat = true;

	skel->animation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f,0.f,0.f)));
	skel->animation.keyFramePositions.push_back(KeyFramePosition(6, glm::vec3(20.f, 0.f, 0.f)));

	skel->animation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f))));

	skel->animation.keyFrameScales.push_back(KeyFrameScale(0, 1.f));


	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(5, "Test Message"));
}

void cGameEngine::SwapPlayer(unsigned int newPlayerNum) {
	if (newPlayerNum >= ::ballEntites.size()) {
		// We don't have that number. Ignore.
		return;
	}
	::g_pPlayerEntity->mesh->textureNames[0] = ::player_inactive_texture;
	::g_pPlayerEntity = ::ballEntites[newPlayerNum];
	::player_inactive_texture = ::g_pPlayerEntity->mesh->textureNames[0];
	::g_pPlayerEntity->mesh->textureNames[0] = "cue.bmp";
}