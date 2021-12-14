#include "cPlayer.h"

#define LeftBound  950.f
#define RightBound -950.f
glm::vec3 currentPos;

cPlayer::cPlayer() {
	mesh = new cMesh();
	mesh->bDontLight = true;
	mesh->bUseWholeObjectDiffuseColour = true;
	mesh->wholeObjectSpecularRGB = glm::vec3(0.f, 1.f, 0.f);
	mesh->wholeObjectDiffuseRGBA = glm::vec4(0.f, 1.f, 0.f, 1.f);
	mesh->friendlyName = "Player";
	mesh->scale = 1.f;
	mesh->meshName = "Invaders/SpaceInvader_Base_block.ply";

	particleScale = 120.f;
	glm::vec3 position = glm::vec3(mesh->positionXYZ.x, mesh->positionXYZ.y + particleScale, mesh->positionXYZ.z);
	particle = new cParticle(position, 1.f, particleScale);
	particle->SetDamping(0.9f);
	particle->type = mesh->friendlyName;
	particle->owner = this;
	worldSpace->_world->AddParticle(particle);
	debugMesh = new cMesh();
	debugMesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
	debugMesh->bIsWireframe = true;
	debugMesh->bUseObjectDebugColour = true;
	debugMesh->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	debugMesh->bDontLight = true;
	debugMesh->scale = particleScale;
	debugMesh->positionXYZ = mesh->positionXYZ;

	bullet = new cBullet();
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
	if (bullet) {
		bullet->Update(deltaTime);
	}
}

void cPlayer::MoveLeft() {
	movingLeft = true;
	particle->SetVelocity(glm::vec3(300.f, 0.f, 0.f));
}

void cPlayer::MoveRight() {
	movingRight = true;
	particle->SetVelocity(glm::vec3(-300.f, 0.f, 0.f));
}

void cPlayer::Stop(int direction) {
	if (direction == 0) {
		movingLeft = false;
		if (particle->GetVelocity().x > 0.f && movingRight) {
			particle->SetVelocity(glm::vec3(-300.f, 0.f, 0.f));
		}
	}
	if (direction == 1) {
		movingRight = false;
		if (particle->GetVelocity().x < 0.f && movingLeft) {
			particle->SetVelocity(glm::vec3(300.f, 0.f, 0.f));
		}
	}
	if (!movingLeft && !movingRight) {
		particle->SetVelocity(glm::vec3(0.f, 0.f, 0.f));
	}
}

void cPlayer::Fire() {
	if (bullet->particle->GetPosition().y > 3000.f) {
		// Bullet isn't active. They can fire again.
		bullet->particle->SetPosition(this->particle->GetPosition() + glm::vec3(0.f, 10.f, 0.f));
		bullet->particle->SetVelocity(glm::vec3(0.f, 1000.f, 0.f));
	}
	// If bullet is in play don't do anything.
}