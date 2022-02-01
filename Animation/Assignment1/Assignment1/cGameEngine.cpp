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

	skel->animations.currentTime = 0;
	skel->animations.duration = 30.0f;
	skel->animations.playing = true;
	skel->animations.speed = 1.f;
	skel->animations.repeat = true;

	Animation skelAnimation;
	skelAnimation.currentTime = 0;
	skelAnimation.duration = 30.0f;
	skelAnimation.playing = true;
	skelAnimation.speed = 1.f;
	skelAnimation.repeat = true;

	// Skeleton Animations.
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(-17.f,0.f,0.f)));
	skelAnimation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f))));
	skelAnimation.keyFrameScales.push_back(KeyFrameScale(0, 2.f));

	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(0.5, new cCommand_ConsoleOutput("<Loop Start>")));
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(1, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseOut))));		// Postion
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(5.5, new cCommand_PlaySound(2)));
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(6, glm::vec3(0.f, 0.f, 0.f),EasingType::EaseOut));

	//Break up animation here.
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(6, new cCommand_ColourChange(skelMesh, true)));
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(8, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseInOut))));		// Rotation + Scale
	skelAnimation.keyFrameScales.push_back(KeyFrameScale(8, 2.f));
	skelAnimation.keyFrameRotations.push_back(KeyFrameRotation(8, glm::quat(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f))));
	skelAnimation.keyFrameScales.push_back(KeyFrameScale(8.5, 1.85f, EasingType::EaseInOut));
	skelAnimation.keyFrameRotations.push_back(KeyFrameRotation(9, glm::quat(glm::vec3(0.f, 0.f, 0.f)), EasingType::EaseInOut));
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(9, new cCommand_ColourChange(skelMesh, true)));
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(10, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseInOut))));	// Position
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(10, glm::vec3(0.f, 0.f, 0.f)));
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(12, glm::vec3(0.f, 0.f, 8.f), EasingType::EaseInOut));
	skelAnimation.keyFrameScales.push_back(KeyFrameScale(12, 1.85f));

	// Break up animation here	
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(12, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::None))));			// Position + Scale
	skelAnimation.keyFrameScales.push_back(KeyFrameScale(12.5, 3.f));
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(12.75, glm::vec3(0.f, 1.f, 8.f)));
	skelAnimation.keyFrameScales.push_back(KeyFrameScale(13, 2.f));
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(13, glm::vec3(0.f, 0.f, 8.f)));

	// Break up animation here
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(13, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseIn))));		// Position
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(15, glm::vec3(0.f, 0.f, 7.f), EasingType::EaseIn));
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(15, new cCommand_ColourChange(skelMesh, true)));
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(16, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseIn))));		// Position
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(16, glm::vec3(0.f, 0.f, 7.f)));
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(18, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseOut))));		// Rotation
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(18, glm::vec3(0.f, 0.f, 6.f), EasingType::EaseIn));
	skelAnimation.keyFrameRotations.push_back(KeyFrameRotation(18, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	skelAnimation.keyFrameRotations.push_back(KeyFrameRotation(18.01, glm::quat(glm::vec3(0.f, glm::pi<float>() / 100.f, 0.f))));
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(18.4, glm::vec3(0.f, 0.f, 6.f)));
	skelAnimation.keyFrameRotations.push_back(KeyFrameRotation(18.5, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f)), EasingType::EaseOut));
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(18.5, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::None))));		// Position
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(20, new cCommand_ColourChange(skelMesh, true)));
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(20, glm::vec3(0.f, 0.f, 2.f)));
	skelAnimation.keyFrameScales.push_back(KeyFrameScale(20, 2.f));
	skelAnimation.keyFrameScales.push_back(KeyFrameScale(20.1, 0.f));
	skelAnimation.keyFramePositions.push_back(KeyFramePosition(21, glm::vec3(-20.f, 0.f, 0.f)));
	skelAnimation.keyFrameEvents.push_back(KeyFrameEvent(29.5, new cCommand_ConsoleOutput("<Loop End>")));

	skelAnimation.keyFramePositions.push_back(KeyFramePosition(30, glm::vec3(-20.f, 0.f, 0.f)));
	skelAnimation.keyFrameRotations.push_back(KeyFrameRotation(30, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	skelAnimation.keyFrameScales.push_back(KeyFrameScale(30, 0.f));

	skel->animations.animations.push_back(skelAnimation);

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

	bush->animations.currentTime = 0;
	bush->animations.duration = 30.0f;
	bush->animations.playing = true;
	bush->animations.speed = 1.f;
	bush->animations.repeat = true;

	Animation bushAnimation;
	bushAnimation.currentTime = 0;
	bushAnimation.duration = 30.0f;
	bushAnimation.playing = true;
	bushAnimation.speed = 1.f;
	bushAnimation.repeat = true;

	// Bush Animation

	bushAnimation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f, 0.f, 15.f)));
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bushAnimation.keyFrameScales.push_back(KeyFrameScale(0, 0.5f));

	bushAnimation.keyFrameEvents.push_back(KeyFrameEvent(3.75, new cCommand_ColourChange(bushMesh, false, GetEasingColour(EasingType::None))));		// Rotation
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(3.75f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bushAnimation.keyFrameEvents.push_back(KeyFrameEvent(4, new cCommand_PlaySound(3)));
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(4.25f, glm::quat(glm::vec3(0.f, 0.f, glm::pi<float>() / 50.f))));
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(4.75f, glm::quat(glm::vec3(0.f, 0.f, -glm::pi<float>() / 50.f))));
	bushAnimation.keyFrameEvents.push_back(KeyFrameEvent(5.25, new cCommand_ColourChange(bushMesh, true)));
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(5.25f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));

	// Break Up animation here
	bushAnimation.keyFrameEvents.push_back(KeyFrameEvent(11.2, new cCommand_PlaySound(3)));
	bushAnimation.keyFrameEvents.push_back(KeyFrameEvent(12, new cCommand_ColourChange(bushMesh, false, GetEasingColour(EasingType::EaseIn))));		// Rotation
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(12.f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(12.5f, glm::quat(glm::vec3(0.f, 0.f, glm::pi<float>() / 50.f)), EasingType::EaseIn));
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(13.f, glm::quat(glm::vec3(0.f, 0.f, -glm::pi<float>() / 50.f))));
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(13.5f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bushAnimation.keyFrameEvents.push_back(KeyFrameEvent(13.5, new cCommand_ColourChange(bushMesh, true)));

	bushAnimation.keyFramePositions.push_back(KeyFramePosition(30, glm::vec3(0.f, 0.f, 15.f)));
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(30.f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bushAnimation.keyFrameScales.push_back(KeyFrameScale(30, 0.5f));

	bush->animations.animations.push_back(bushAnimation);

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

	exc->animations.currentTime = 0;
	exc->animations.duration = 30.0f;
	exc->animations.playing = true;
	exc->animations.speed = 1.f;
	exc->animations.repeat = true;

	Animation excAnimation;
	excAnimation.currentTime = 0;
	excAnimation.duration = 30.0f;
	excAnimation.playing = true;
	excAnimation.speed = 1.f;
	excAnimation.repeat = true;

	//Exclamation Animation
	excAnimation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f, 6.f, 8.f)));
	excAnimation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	excAnimation.keyFrameScales.push_back(KeyFrameScale(0, 0.f));

	excAnimation.keyFrameScales.push_back(KeyFrameScale(12, 0.f));
	excAnimation.keyFrameEvents.push_back(KeyFrameEvent(12, new cCommand_ColourChange(excMesh, false, GetEasingColour(EasingType::EaseOut))));		// Scale
	excAnimation.keyFrameEvents.push_back(KeyFrameEvent(12.2, new cCommand_PlaySound(0)));
	excAnimation.keyFrameEvents.push_back(KeyFrameEvent(12.5, new cCommand_ColourChange(excMesh, true)));
	excAnimation.keyFrameScales.push_back(KeyFrameScale(12.5, 5.f, EasingType::EaseOut));
	excAnimation.keyFrameScales.push_back(KeyFrameScale(13, 5.f));
	excAnimation.keyFrameEvents.push_back(KeyFrameEvent(13, new cCommand_ColourChange(excMesh, false, GetEasingColour(EasingType::None))));		// Scale
	excAnimation.keyFrameEvents.push_back(KeyFrameEvent(13.5, new cCommand_ColourChange(excMesh, true)));
	excAnimation.keyFrameScales.push_back(KeyFrameScale(13.5, 0.f));

	excAnimation.keyFramePositions.push_back(KeyFramePosition(30, glm::vec3(0.f, 6.f, 8.f)));
	excAnimation.keyFrameRotations.push_back(KeyFrameRotation(30, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	excAnimation.keyFrameScales.push_back(KeyFrameScale(30, 0.f));

	exc->animations.animations.push_back(excAnimation);

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

	blood->animations.currentTime = 0;
	blood->animations.duration = 30.0f;
	blood->animations.playing = true;
	blood->animations.speed = 1.f;
	blood->animations.repeat = true;

	Animation bloodAnimation;
	bloodAnimation.currentTime = 0;
	bloodAnimation.duration = 30.0f;
	bloodAnimation.playing = true;
	bloodAnimation.speed = 1.f;
	bloodAnimation.repeat = true;

	//Blood Animation
	bloodAnimation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f, 3.f, 5.f)));
	bloodAnimation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	bloodAnimation.keyFrameScales.push_back(KeyFrameScale(0, 0.f));

	bloodAnimation.keyFrameEvents.push_back(KeyFrameEvent(1, new cCommand_ColourChange(bloodMesh, false, GetEasingColour(EasingType::None))));		// Position
	bloodAnimation.keyFrameEvents.push_back(KeyFrameEvent(18.5, new cCommand_PlaySound(1)));
	bloodAnimation.keyFrameScales.push_back(KeyFrameScale(19, 0.f));
	bloodAnimation.keyFramePositions.push_back(KeyFramePosition(19, glm::vec3(0.f, 3.f, 5.f)));
	bloodAnimation.keyFrameEvents.push_back(KeyFrameEvent(19, new cCommand_ColourChange(bloodMesh, false, GetEasingColour(EasingType::EaseIn))));		//Scale
	bloodAnimation.keyFramePositions.push_back(KeyFramePosition(20, glm::vec3(0.f, 3.f, -10.f)));
	bloodAnimation.keyFrameScales.push_back(KeyFrameScale(20.5, 20.f, EasingType::EaseIn));
	bloodAnimation.keyFrameEvents.push_back(KeyFrameEvent(20.5, new cCommand_ColourChange(bloodMesh, false, GetEasingColour(EasingType::EaseOut))));		//Position

	bloodAnimation.keyFramePositions.push_back(KeyFramePosition(30, glm::vec3(0.f, -10.f, -10.f), EasingType::EaseOut));
	bloodAnimation.keyFrameRotations.push_back(KeyFrameRotation(30, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	bloodAnimation.keyFrameScales.push_back(KeyFrameScale(30, 20.f));

	blood->animations.animations.push_back(bloodAnimation);
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

