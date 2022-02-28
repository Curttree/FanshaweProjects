#include "cGameEngine.h"
#include "globals.h"

#include "cBone.h"

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

Transform CreateTransformFromBone(cBone* bone)
{
	Transform transform;
	transform.position = bone->GetPosition();
	transform.scale = bone->GetScale();
	transform.rotation = bone->GetRotation();
	return transform;
}

void cGameEngine::LoadAnimationAssignmentTwoScene() {
	//Skeleton 
	cEntity* trent = entityManager.CreateEntity();

	cMesh* tentacleMesh = new cMesh();
	tentacleMesh->meshName = "tentacle.ply";
	tentacleMesh->textureNames[0] = "white.bmp";
	tentacleMesh->textureRatios[0] = 1.f;
	tentacleMesh->scale = 1.0f;
	tentacleMesh->positionXYZ = glm::vec3(0.f, -10.f, 50.f);
	tentacleMesh->bUseWholeObjectDiffuseColour = false;
	tentacleMesh->bDontLight = true;
	tentacleMesh->bUseObjectDebugColour = false;
	tentacleMesh->friendlyName = "Trent";
	tentacleMesh->bUseBones = true;


	trent->position = tentacleMesh->positionXYZ;
	trent->rotation = tentacleMesh->orientationXYZ;
	trent->scale = tentacleMesh->scale;

	// Create a bone hierarchy for 4 bones for our 4-tier-cone.obj model
	cBone* boneA = new cBone();
	cBone* boneB = new cBone();
	cBone* boneC = new cBone();
	cBone* boneD = new cBone();

	boneA->index = 0;
	boneB->index = 1;
	boneC->index = 2;
	boneD->index = 3;

	boneA->SetPosition(glm::vec3(0.f, 0.f, 0.f));
	boneB->SetPosition(glm::vec3(0.f, 4.f, 0.f));
	boneC->SetPosition(glm::vec3(0.f, 4.f, 0.f));
	boneD->SetPosition(glm::vec3(0.f, 4.f, 0.f));

	boneA->AddChild(boneB);
	boneB->SetParent(boneA);

	boneB->AddChild(boneC);
	boneC->SetParent(boneB);

	boneC->AddChild(boneD);
	boneD->SetParent(boneC);

	tentacleMesh->bones = new cBoneHierarchy();
	tentacleMesh->bones->rootBone = boneA;
	tentacleMesh->bones->bones.push_back(boneA);
	tentacleMesh->bones->bones.push_back(boneB);
	tentacleMesh->bones->bones.push_back(boneC);
	tentacleMesh->bones->bones.push_back(boneD);

	for (int i = 0; i < tentacleMesh->bones->bones.size(); ++i)
	{
		Transform boneTransform = CreateTransformFromBone(tentacleMesh->bones->bones[i]);
		tentacleMesh->bones->currentTransforms.push_back(boneTransform);
	}

	trent->mesh = tentacleMesh;
	::g_vec_pMeshes.push_back(tentacleMesh);

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

	skel->animations.playing = true;
	skel->animations.speed = 1.f;
	skel->animations.repeat = true;

	Animation skelEntranceAnimation;
	skelEntranceAnimation.currentTime = 0;
	skelEntranceAnimation.duration = 6.0f;
	skelEntranceAnimation.shouldPlay = true;
	skelEntranceAnimation.speed = 1.f;
	skelEntranceAnimation.repeat = false;

	Animation skelInvestigateAnimation;
	skelInvestigateAnimation.currentTime = 0;
	skelInvestigateAnimation.duration = 6.0f;
	skelInvestigateAnimation.shouldPlay = true;
	skelInvestigateAnimation.speed = 1.f;
	skelInvestigateAnimation.repeat = false;

	Animation skelScaredAnimation;
	skelScaredAnimation.currentTime = 0;
	skelScaredAnimation.duration = 1.0f;
	skelScaredAnimation.shouldPlay = true;
	skelScaredAnimation.speed = 1.f;
	skelScaredAnimation.repeat = false;

	Animation skelRunAwayAnimation;
	skelRunAwayAnimation.currentTime = 0;
	skelRunAwayAnimation.duration = 17.0f;
	skelRunAwayAnimation.shouldPlay = true;
	skelRunAwayAnimation.speed = 1.f;
	skelRunAwayAnimation.repeat = false;

	// Skeleton Animations - Entrance.
	skelEntranceAnimation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(-17.f,0.f,0.f)));
	skelEntranceAnimation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f))));
	skelEntranceAnimation.keyFrameScales.push_back(KeyFrameScale(0, 2.f));

	skelEntranceAnimation.keyFrameEvents.push_back(KeyFrameEvent(0.5, new cCommand_ConsoleOutput("<Skeleton Loop Start>")));
	skelEntranceAnimation.keyFrameEvents.push_back(KeyFrameEvent(1, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseOut))));		// Postion
	skelEntranceAnimation.keyFrameEvents.push_back(KeyFrameEvent(5.5, new cCommand_PlaySound(2)));

	skelEntranceAnimation.keyFramePositions.push_back(KeyFramePosition(6, glm::vec3(0.f, 0.f, 0.f),EasingType::EaseOut));
	skelEntranceAnimation.keyFrameRotations.push_back(KeyFrameRotation(6, glm::quat(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f))));
	skelEntranceAnimation.keyFrameScales.push_back(KeyFrameScale(6, 2.f));

	// Skeleton Animations - Investigate noise.
	skelInvestigateAnimation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f, 0.f, 0.f), EasingType::EaseOut));
	skelInvestigateAnimation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f))));
	skelInvestigateAnimation.keyFrameScales.push_back(KeyFrameScale(0, 2.f));

	skelInvestigateAnimation.keyFrameEvents.push_back(KeyFrameEvent(0.1, new cCommand_ColourChange(skelMesh, true)));
	skelInvestigateAnimation.keyFrameEvents.push_back(KeyFrameEvent(2, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseInOut))));		// Rotation + Scale + Position
	skelInvestigateAnimation.keyFrameScales.push_back(KeyFrameScale(2, 2.f));
	skelInvestigateAnimation.keyFrameRotations.push_back(KeyFrameRotation(2, glm::quat(glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f))));
	skelInvestigateAnimation.keyFrameScales.push_back(KeyFrameScale(2.5, 1.85f, EasingType::EaseInOut));
	skelInvestigateAnimation.keyFrameRotations.push_back(KeyFrameRotation(3, glm::quat(glm::vec3(0.f, 0.f, 0.f)), EasingType::EaseInOut));
	skelInvestigateAnimation.keyFramePositions.push_back(KeyFramePosition(4, glm::vec3(0.f, 0.f, 0.f)));

	skelInvestigateAnimation.keyFramePositions.push_back(KeyFramePosition(6, glm::vec3(0.f, 0.f, 8.f), EasingType::EaseInOut));
	skelInvestigateAnimation.keyFrameRotations.push_back(KeyFrameRotation(6, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	skelInvestigateAnimation.keyFrameScales.push_back(KeyFrameScale(6, 1.85f));

	// Skeleton Animations - Scared jump.
	skelScaredAnimation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f, 0.f, 8.f), EasingType::EaseInOut));
	skelScaredAnimation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	skelScaredAnimation.keyFrameScales.push_back(KeyFrameScale(0, 1.85f));

	skelScaredAnimation.keyFrameEvents.push_back(KeyFrameEvent(0.1, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::None))));			// Position + Scale
	skelScaredAnimation.keyFrameScales.push_back(KeyFrameScale(0.5, 3.f));
	skelScaredAnimation.keyFramePositions.push_back(KeyFramePosition(0.75, glm::vec3(0.f, 1.f, 8.f)));

	skelScaredAnimation.keyFrameScales.push_back(KeyFrameScale(1, 2.f));
	skelScaredAnimation.keyFrameRotations.push_back(KeyFrameRotation(1, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	skelScaredAnimation.keyFramePositions.push_back(KeyFramePosition(1, glm::vec3(0.f, 0.f, 8.f)));

	// Skeleton Animations - Run Away.
	skelRunAwayAnimation.keyFrameScales.push_back(KeyFrameScale(0, 2.f));
	skelRunAwayAnimation.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	skelRunAwayAnimation.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f, 0.f, 8.f)));

	skelRunAwayAnimation.keyFrameEvents.push_back(KeyFrameEvent(0.1, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseIn))));		// Position
	skelRunAwayAnimation.keyFramePositions.push_back(KeyFramePosition(2, glm::vec3(0.f, 0.f, 7.f), EasingType::EaseIn));
	skelRunAwayAnimation.keyFrameEvents.push_back(KeyFrameEvent(2, new cCommand_ColourChange(skelMesh, true)));
	skelRunAwayAnimation.keyFrameEvents.push_back(KeyFrameEvent(3, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseIn))));		// Position
	skelRunAwayAnimation.keyFramePositions.push_back(KeyFramePosition(3, glm::vec3(0.f, 0.f, 7.f)));
	skelRunAwayAnimation.keyFrameEvents.push_back(KeyFrameEvent(5, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::EaseOut))));		// Rotation
	skelRunAwayAnimation.keyFramePositions.push_back(KeyFramePosition(5, glm::vec3(0.f, 0.f, 6.f), EasingType::EaseIn));
	skelRunAwayAnimation.keyFrameRotations.push_back(KeyFrameRotation(5, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	skelRunAwayAnimation.keyFrameRotations.push_back(KeyFrameRotation(5.01, glm::quat(glm::vec3(0.f, glm::pi<float>() / 100.f, 0.f))));
	skelRunAwayAnimation.keyFramePositions.push_back(KeyFramePosition(5.4, glm::vec3(0.f, 0.f, 6.f)));
	skelRunAwayAnimation.keyFrameRotations.push_back(KeyFrameRotation(5.5, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f)), EasingType::EaseOut));
	skelRunAwayAnimation.keyFrameEvents.push_back(KeyFrameEvent(5.5, new cCommand_ColourChange(skelMesh, false, GetEasingColour(EasingType::None))));		// Position
	skelRunAwayAnimation.keyFrameEvents.push_back(KeyFrameEvent(7, new cCommand_ColourChange(skelMesh, true)));
	skelRunAwayAnimation.keyFramePositions.push_back(KeyFramePosition(7, glm::vec3(0.f, 0.f, 2.f)));
	skelRunAwayAnimation.keyFrameScales.push_back(KeyFrameScale(7, 2.f));
	skelRunAwayAnimation.keyFrameScales.push_back(KeyFrameScale(7.1, 0.f));
	skelRunAwayAnimation.keyFramePositions.push_back(KeyFramePosition(8, glm::vec3(-20.f, 0.f, 0.f)));
	skelRunAwayAnimation.keyFrameEvents.push_back(KeyFrameEvent(16.5, new cCommand_ConsoleOutput("<Skeleton Loop End>")));

	skelRunAwayAnimation.keyFramePositions.push_back(KeyFramePosition(17, glm::vec3(-20.f, 0.f, 0.f)));
	skelRunAwayAnimation.keyFrameRotations.push_back(KeyFrameRotation(17, glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f))));
	skelRunAwayAnimation.keyFrameScales.push_back(KeyFrameScale(17, 0.f));

	skel->animations.animations.push_back(skelEntranceAnimation);
	skel->animations.animations.push_back(skelInvestigateAnimation);
	skel->animations.animations.push_back(skelScaredAnimation);
	skel->animations.animations.push_back(skelRunAwayAnimation);

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

	bush->animations.playing = true;
	bush->animations.speed = 1.f;
	bush->animations.repeat = true;

	Animation bushAnimation;
	bushAnimation.currentTime = 0;
	bushAnimation.duration = 6.0f;
	bushAnimation.shouldPlay = true;
	bushAnimation.speed = 1.f;
	bushAnimation.repeat = false;

	Animation bushAnimationEase;
	bushAnimationEase.currentTime = 0;
	bushAnimationEase.duration = 24.0f;
	bushAnimationEase.shouldPlay = true;
	bushAnimationEase.speed = 1.f;
	bushAnimationEase.repeat = false;

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

	bushAnimation.keyFramePositions.push_back(KeyFramePosition(6, glm::vec3(0.f, 0.f, 15.f)));
	bushAnimation.keyFrameRotations.push_back(KeyFrameRotation(6, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bushAnimation.keyFrameScales.push_back(KeyFrameScale(6, 0.5f));

	// Second shake. This time, ease in.

	bushAnimationEase.keyFramePositions.push_back(KeyFramePosition(0, glm::vec3(0.f, 0.f, 15.f)));
	bushAnimationEase.keyFrameRotations.push_back(KeyFrameRotation(0, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bushAnimationEase.keyFrameScales.push_back(KeyFrameScale(0, 0.5f));

	bushAnimationEase.keyFrameEvents.push_back(KeyFrameEvent(5.2, new cCommand_PlaySound(3)));
	bushAnimationEase.keyFrameEvents.push_back(KeyFrameEvent(6, new cCommand_ColourChange(bushMesh, false, GetEasingColour(EasingType::EaseIn))));		// Rotation
	bushAnimationEase.keyFrameRotations.push_back(KeyFrameRotation(6.f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bushAnimationEase.keyFrameRotations.push_back(KeyFrameRotation(6.5f, glm::quat(glm::vec3(0.f, 0.f, glm::pi<float>() / 50.f)), EasingType::EaseIn));
	bushAnimationEase.keyFrameRotations.push_back(KeyFrameRotation(7.f, glm::quat(glm::vec3(0.f, 0.f, -glm::pi<float>() / 50.f))));
	bushAnimationEase.keyFrameRotations.push_back(KeyFrameRotation(7.5f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bushAnimationEase.keyFrameEvents.push_back(KeyFrameEvent(7.5, new cCommand_ColourChange(bushMesh, true)));

	bushAnimationEase.keyFramePositions.push_back(KeyFramePosition(24, glm::vec3(0.f, 0.f, 15.f)));
	bushAnimationEase.keyFrameRotations.push_back(KeyFrameRotation(24.f, glm::quat(glm::vec3(0.f, 0.f, 0.f))));
	bushAnimationEase.keyFrameScales.push_back(KeyFrameScale(24, 0.5f));

	bush->animations.animations.push_back(bushAnimation);
	bush->animations.animations.push_back(bushAnimationEase);

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

	exc->animations.playing = true;
	exc->animations.speed = 1.f;
	exc->animations.repeat = true;

	Animation excAnimation;
	excAnimation.currentTime = 0;
	excAnimation.duration = 30.0f;
	excAnimation.shouldPlay = true;
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

	blood->animations.playing = true;
	blood->animations.speed = 1.f;
	blood->animations.repeat = true;

	Animation bloodAnimation;
	bloodAnimation.currentTime = 0;
	bloodAnimation.duration = 30.0f;
	bloodAnimation.shouldPlay = true;
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

void cGameEngine::CycleActiveSequence(int offset) {
	activeSequenceIndex+= offset;
	if (activeSequenceIndex >= (int)entityManager.GetEntities().size()) {
		activeSequenceIndex = 0;
	}
	else if (activeSequenceIndex < 0) {
		activeSequenceIndex = entityManager.GetEntities().size()-1;
	}
}

std::string cGameEngine::GetActiveSequenceName() {
	// Right now I am using the mesh name. Consider moving the name to the sequence if we wish to break the dependency between the mesh and animation sequence.
	return entityManager.GetEntities()[activeSequenceIndex]->mesh->friendlyName;
}