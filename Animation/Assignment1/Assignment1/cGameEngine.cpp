#include "cGameEngine.h"
#include "globals.h"

// If I am going to continue to add commands, I should switch to a factory.
#include "cCommand_ConsoleOutput.h"
#include "cCommand_ColourChange.h"
#include "cCommand_PlaySound.h"

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
	HandlePlayerInput();
}

void cGameEngine::SetGameSpeed(float speed) {
	gameSpeed /= glm::abs(gameSpeed);
	gameSpeed *= speed;
}
void cGameEngine::ReverseGameSpeed() {
	gameSpeed *= -1.f;
}

glm::vec3 GetEasingColour(EasingType type) {
	switch (type)
	{
	case EaseIn:
		// Red
		return glm::vec3(1.f, 0.f, 0.f);
		break;
	case EaseOut:
		// Yellow
		return glm::vec3(1.f, 1.f, 0.f);
		break;
	case EaseInOut:
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
	if (keys[GLFW_KEY_SPACE]) {
		isPaused = !isPaused;
		keys[GLFW_KEY_SPACE] = false;
	}
	if (keys[GLFW_KEY_R]) {
		ReverseGameSpeed();
		keys[GLFW_KEY_R] = false;
	}
	if (keys[GLFW_KEY_C]) {
		showColourChanges = !showColourChanges;
		keys[GLFW_KEY_C] = false;
	}
	if (keys[GLFW_KEY_M]) {
		isMuted = !isMuted;
		keys[GLFW_KEY_M] = false;
	}
}

void cGameEngine::LoadAnimationAssignmentOneScene() {
	//Skeleton 
	cEntity* skel = entityManager.CreateEntity(); 

	cMesh* skelMesh = new cMesh();
	skelMesh->meshName = "character_skeleton_minion.ply";
	skelMesh->textureNames[0] = "white.bmp";
	skelMesh->textureRatios[0] = 1.f;
	skelMesh->scale = 2.0f;
	skelMesh->positionXYZ = glm::vec3(0.f, 0.f, 0.f); 
	skelMesh->bUseWholeObjectDiffuseColour = true;
	skelMesh->bDontLight = false;
	skelMesh->bUseObjectDebugColour = false;
	skelMesh->friendlyName = "Skeleton";
	skel->mesh = skelMesh;

	::g_vec_pMeshes.push_back(skelMesh);

	skel->animation.currentTime = 0;
	skel->animation.duration = 30.0f;
	skel->animation.playing = true;
	skel->animation.speed = 1.f;
	skel->animation.repeat = true;

	//Bush
	cEntity* bush = entityManager.CreateEntity();

	cMesh* bushMesh = new cMesh();
	bushMesh->meshName = "Buxus_sphere_small.ply";
	bushMesh->textureNames[0] = "leaf.bmp";
	bushMesh->textureRatios[0] = 1.f;
	bushMesh->scale = 1.0f;
	bushMesh->positionXYZ = glm::vec3(0.f, 0.f, 20.f);
	bushMesh->bUseWholeObjectDiffuseColour = true;
	bushMesh->bDontLight = false;
	bushMesh->bUseObjectDebugColour = false;
	bushMesh->friendlyName = "Bush";
	bush->mesh = bushMesh;

	::g_vec_pMeshes.push_back(bushMesh);

	bush->animation.currentTime = 0;
	bush->animation.duration = 30.0f;
	bush->animation.playing = true;
	bush->animation.speed = 1.f;
	bush->animation.repeat = true;

	//exclamation Point
	cEntity* exc = entityManager.CreateEntity();

	cMesh* excMesh = new cMesh();
	excMesh->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
	excMesh->textureNames[8] = "exclaim.bmp";
	excMesh->textureRatios[8] = 1.f;
	excMesh->scale = 1.0f;
	excMesh->positionXYZ = glm::vec3(0.f, 0.f, 20.f);
	excMesh->bUseWholeObjectDiffuseColour = true;
	excMesh->bDontLight = true;
	excMesh->bUseObjectDebugColour = false;
	excMesh->bUseDiscardTransparency = true;
	excMesh->friendlyName = "Exclamation point";
	exc->mesh = excMesh;

	::g_vec_pMeshes.push_back(excMesh);

	exc->animation.currentTime = 0;
	exc->animation.duration = 30.0f;
	exc->animation.playing = true;
	exc->animation.speed = 1.f;
	exc->animation.repeat = true;

	//Blood Spatter
	cEntity* blood = entityManager.CreateEntity();

	cMesh* bloodMesh = new cMesh();
	bloodMesh->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
	bloodMesh->textureNames[8] = "blood.bmp";
	bloodMesh->textureRatios[8] = 1.f;
	bloodMesh->scale = 1.0f;
	bloodMesh->positionXYZ = glm::vec3(0.f, 0.f, 20.f);
	bloodMesh->bUseWholeObjectDiffuseColour = true;
	bloodMesh->bDontLight = true;
	bloodMesh->bUseObjectDebugColour = false;
	bloodMesh->bUseDiscardTransparency = true;
	bloodMesh->friendlyName = "Blood";
	blood->mesh = bloodMesh;

	::g_vec_pMeshes.push_back(bloodMesh);

	blood->animation.currentTime = 0;
	blood->animation.duration = 30.0f;
	blood->animation.playing = true;
	blood->animation.speed = 1.f;
	blood->animation.repeat = true;

	// Skeleton Animations.
	skel->animation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(-17.f,0.f,0.f)));
	skel->animation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f))));
	skel->animation.keyFrameScales.push_back(KeyFrameScale(0, 2.f));

	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(0.5, new cCommand_ConsoleOutput("<Loop Start>")));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(1, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseOut))));		// Postion
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(5.5, new cCommand_PlaySound(2)));
	skel->animation.keyFramePositions.push_back(KeyFramePosition(6, glm::vec3(0.f, 0.f, 0.f),EasingType::EaseOut));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(6, new cCommand_ColourChange(skelMesh, true)));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(8, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseInOut))));		// Rotation + Scale
	skel->animation.keyFrameScales.push_back(KeyFrameScale(8, 2.f));
	skel->animation.keyFrameRotations.push_back(KeyFrameRotation(8, glm::quat(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f))));
	skel->animation.keyFrameScales.push_back(KeyFrameScale(8.5, 1.85f, EasingType::EaseInOut));
	skel->animation.keyFrameRotations.push_back(KeyFrameRotation(9, glm::quat(glm::vec3(0.f, 0.f, 0.f)), EasingType::EaseInOut));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(9, new cCommand_ColourChange(skelMesh, true)));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(10, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseInOut))));	// Position
	skel->animation.keyFramePositions.push_back(KeyFramePosition(10, glm::vec3(0.f, 0.f, 0.f)));
	skel->animation.keyFramePositions.push_back(KeyFramePosition(12, glm::vec3(0.f, 0.f, 8.f), EasingType::EaseInOut));
	skel->animation.keyFrameScales.push_back(KeyFrameScale(12, 1.85f));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(12, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::None))));			// Position + Scale
	skel->animation.keyFrameScales.push_back(KeyFrameScale(12.5, 3.f));
	skel->animation.keyFramePositions.push_back(KeyFramePosition(12.75, glm::vec3(0.f, 1.f, 8.f)));
	skel->animation.keyFrameScales.push_back(KeyFrameScale(13, 2.f));
	skel->animation.keyFramePositions.push_back(KeyFramePosition(13, glm::vec3(0.f, 0.f, 8.f)));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(13, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseIn))));		// Position
	skel->animation.keyFramePositions.push_back(KeyFramePosition(15, glm::vec3(0.f, 0.f, 7.f), EasingType::EaseIn));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(15, new cCommand_ColourChange(skelMesh, true)));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(16, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseIn))));		// Position
	skel->animation.keyFramePositions.push_back(KeyFramePosition(16, glm::vec3(0.f, 0.f, 7.f)));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(18, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseOut))));		// Rotation
	skel->animation.keyFramePositions.push_back(KeyFramePosition(18, glm::vec3(0.f, 0.f, 6.f), EasingType::EaseIn));
	skel->animation.keyFrameRotations.push_back(KeyFrameRotation(18, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	skel->animation.keyFrameRotations.push_back(KeyFrameRotation(18.01, glm::quat(glm::vec3(0.f, glm::pi<float>() / 100.f, 0.f))));
	skel->animation.keyFramePositions.push_back(KeyFramePosition(18.4, glm::vec3(0.f, 0.f, 6.f)));
	skel->animation.keyFrameRotations.push_back(KeyFrameRotation(18.5, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f)), EasingType::EaseOut));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(18.5, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::None))));		// Position
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(20, new cCommand_ColourChange(skelMesh, true)));
	skel->animation.keyFramePositions.push_back(KeyFramePosition(20, glm::vec3(0.f, 0.f, 2.f)));
	skel->animation.keyFrameScales.push_back(KeyFrameScale(20, 2.f));
	skel->animation.keyFrameScales.push_back(KeyFrameScale(20.1, 0.f));
	skel->animation.keyFramePositions.push_back(KeyFramePosition(21, glm::vec3(-20.f, 0.f, 0.f)));
	skel->animation.keyFrameEvents.push_back(KeyFrameEvent(29.5, new cCommand_ConsoleOutput("<Loop End>")));

	skel->animation.keyFramePositions.push_back(KeyFramePosition(30, glm::vec3(-20.f, 0.f, 0.f)));
	skel->animation.keyFrameRotations.push_back(KeyFrameRotation(30, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	skel->animation.keyFrameScales.push_back(KeyFrameScale(30, 0.f));

	// Bush Animation

	bush->animation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f, 0.f, 15.f)));
	bush->animation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bush->animation.keyFrameScales.push_back(KeyFrameScale(0, 0.5f));

	bush->animation.keyFrameEvents.push_back(KeyFrameEvent(3.75, new cCommand_ColourChange(bushMesh, false, GetEasingColour(EasingType::None))));		// Rotation
	bush->animation.keyFrameRotations.push_back(KeyFrameRotation(3.75f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bush->animation.keyFrameEvents.push_back(KeyFrameEvent(4, new cCommand_PlaySound(3)));
	bush->animation.keyFrameRotations.push_back(KeyFrameRotation(4.25f, glm::quat(glm::vec3(0.f, 0.f, glm::pi<float>() / 50.f))));
	bush->animation.keyFrameRotations.push_back(KeyFrameRotation(4.75f, glm::quat(glm::vec3(0.f, 0.f, -glm::pi<float>() / 50.f))));
	bush->animation.keyFrameEvents.push_back(KeyFrameEvent(5.25, new cCommand_ColourChange(bushMesh, true)));
	bush->animation.keyFrameRotations.push_back(KeyFrameRotation(5.25f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bush->animation.keyFrameEvents.push_back(KeyFrameEvent(11.2, new cCommand_PlaySound(3)));
	bush->animation.keyFrameEvents.push_back(KeyFrameEvent(12, new cCommand_ColourChange(bushMesh, false, GetEasingColour(EasingType::EaseIn))));		// Rotation
	bush->animation.keyFrameRotations.push_back(KeyFrameRotation(12.f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bush->animation.keyFrameRotations.push_back(KeyFrameRotation(12.5f, glm::quat(glm::vec3(0.f, 0.f, glm::pi<float>() / 50.f)), EasingType::EaseIn));
	bush->animation.keyFrameRotations.push_back(KeyFrameRotation(13.f, glm::quat(glm::vec3(0.f, 0.f, -glm::pi<float>() / 50.f))));
	bush->animation.keyFrameRotations.push_back(KeyFrameRotation(13.5f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bush->animation.keyFrameEvents.push_back(KeyFrameEvent(13.5, new cCommand_ColourChange(bushMesh, true)));

	bush->animation.keyFramePositions.push_back(KeyFramePosition(30, glm::vec3(0.f, 0.f, 15.f)));
	bush->animation.keyFrameRotations.push_back(KeyFrameRotation(30.f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bush->animation.keyFrameScales.push_back(KeyFrameScale(30, 0.5f));

	//Exclamation Animation
	exc->animation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f, 6.f, 8.f)));
	exc->animation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	exc->animation.keyFrameScales.push_back(KeyFrameScale(0, 0.f));

	exc->animation.keyFrameScales.push_back(KeyFrameScale(12, 0.f));
	exc->animation.keyFrameEvents.push_back(KeyFrameEvent(12, new cCommand_ColourChange(excMesh, false, GetEasingColour(EasingType::EaseOut))));		// Scale
	exc->animation.keyFrameEvents.push_back(KeyFrameEvent(12.2, new cCommand_PlaySound(0)));
	exc->animation.keyFrameEvents.push_back(KeyFrameEvent(12.5, new cCommand_ColourChange(excMesh, true)));
	exc->animation.keyFrameScales.push_back(KeyFrameScale(12.5, 5.f, EasingType::EaseOut));
	exc->animation.keyFrameScales.push_back(KeyFrameScale(13, 5.f));
	exc->animation.keyFrameEvents.push_back(KeyFrameEvent(13, new cCommand_ColourChange(excMesh, false, GetEasingColour(EasingType::None))));		// Scale
	exc->animation.keyFrameEvents.push_back(KeyFrameEvent(13.5, new cCommand_ColourChange(excMesh, true)));
	exc->animation.keyFrameScales.push_back(KeyFrameScale(13.5, 0.f));

	exc->animation.keyFramePositions.push_back(KeyFramePosition(30, glm::vec3(0.f, 6.f, 8.f)));
	exc->animation.keyFrameRotations.push_back(KeyFrameRotation(30, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	exc->animation.keyFrameScales.push_back(KeyFrameScale(30, 0.f));

	//Blood Animation
	blood->animation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f, 3.f, 5.f)));
	blood->animation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	blood->animation.keyFrameScales.push_back(KeyFrameScale(0, 0.f));

	blood->animation.keyFrameEvents.push_back(KeyFrameEvent(1, new cCommand_ColourChange(bloodMesh, false, GetEasingColour(EasingType::None))));		// Position
	blood->animation.keyFrameEvents.push_back(KeyFrameEvent(18.5, new cCommand_PlaySound(1)));
	blood->animation.keyFrameScales.push_back(KeyFrameScale(19, 0.f));
	blood->animation.keyFramePositions.push_back(KeyFramePosition(19, glm::vec3(0.f, 3.f, 5.f)));
	blood->animation.keyFrameEvents.push_back(KeyFrameEvent(19, new cCommand_ColourChange(bloodMesh, false, GetEasingColour(EasingType::EaseIn))));		//Scale
	blood->animation.keyFramePositions.push_back(KeyFramePosition(20, glm::vec3(0.f, 3.f, -10.f)));
	blood->animation.keyFrameScales.push_back(KeyFrameScale(20.5, 20.f, EasingType::EaseIn));
	blood->animation.keyFrameEvents.push_back(KeyFrameEvent(20.5, new cCommand_ColourChange(bloodMesh, false, GetEasingColour(EasingType::EaseOut))));		//Position

	blood->animation.keyFramePositions.push_back(KeyFramePosition(30, glm::vec3(0.f, -10.f, -10.f), EasingType::EaseOut));
	blood->animation.keyFrameRotations.push_back(KeyFrameRotation(30, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	blood->animation.keyFrameScales.push_back(KeyFrameScale(30, 20.f));
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

