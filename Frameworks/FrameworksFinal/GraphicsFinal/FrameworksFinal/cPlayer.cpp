#include "cPlayer.h"

#define LeftBound  950.f
#define RightBound -950.f
glm::vec3 currentPos;

cPlayer::cPlayer() {
	mesh = new cMesh();
	mesh->bDontLight = true;
	mesh->bUseWholeObjectDiffuseColour = true;
	mesh->wholeObjectSpecularRGB = glm::vec3(1.f, 1.f, 1.f);
	mesh->wholeObjectDiffuseRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
	mesh->friendlyName = "Player";
	mesh->scale = 1.f;
	mesh->meshName = "Invaders/SpaceInvader_Base_block.ply";

	particleScale = 1.f;
	glm::vec3 position = glm::vec3(mesh->positionXYZ.x, mesh->positionXYZ.y + particleScale, mesh->positionXYZ.z);
	particle = new cParticle(position, 1.f, particleScale);
	particle->SetDamping(0.9f);
	worldSpace->_world->AddParticle(particle);
	debugMesh = new cMesh();
	debugMesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
	debugMesh->bIsWireframe = true;
	debugMesh->bUseObjectDebugColour = true;
	debugMesh->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	debugMesh->bDontLight = true;
	debugMesh->scale = particleScale;
	debugMesh->positionXYZ = mesh->positionXYZ;
}

void cPlayer::Update(float deltaTime)  {
	currentPos = particle->GetPosition();
	if (mesh->positionXYZ.x > LeftBound) {
		particle->SetPosition(glm::vec3(LeftBound, currentPos.y, currentPos.z));
	}
	if (mesh->positionXYZ.x < RightBound) {
		particle->SetPosition(glm::vec3(RightBound, currentPos.y, currentPos.z));
	}
	cEntity::Update(deltaTime);
}

void cPlayer::MoveLeft() {
	particle->SetVelocity(glm::vec3(300.f, 0.f, 0.f));
}

void cPlayer::MoveRight() {
	particle->SetVelocity(glm::vec3(-300.f, 0.f, 0.f));
}

void cPlayer::Stop() {
	particle->SetVelocity(glm::vec3(0.f, 0.f, 0.f));
}