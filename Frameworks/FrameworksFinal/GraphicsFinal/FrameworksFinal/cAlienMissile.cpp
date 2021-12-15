#include "cAlienMissile.h"

#define LOWER_LIMIT -1500.f

cAlienMissile::cAlienMissile() {
	mesh = new cMesh();
	mesh->bDontLight = true;
	mesh->bUseWholeObjectDiffuseColour = true;
	mesh->wholeObjectSpecularRGB = glm::vec3(1.f, 1.f, 1.f);
	mesh->wholeObjectDiffuseRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
	mesh->friendlyName = "Missile";
	mesh->scale = 1.f;
	mesh->meshName = "Invaders/SpaceInvader_Bomb_pose_1.ply";
	mesh->positionXYZ = glm::vec3(0.f, -10000.f, 0.f);

	particleScale = 40.f;
	glm::vec3 position = mesh->positionXYZ;
	particle = new cParticle(position, 1.f, particleScale);
	particle->SetDamping(1.0f);
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
}


void cAlienMissile::Update(float deltaTime) {
	timeSinceLastChange += deltaTime;
	if (timeSinceLastChange >= animTime) {
		timeSinceLastChange = 0.f;
		if (currentModel == 3) {
			currentModel = 0;
		}
		else {
			currentModel++;
		}
		mesh->meshName = models[currentModel];
	}
	cEntity::Update(deltaTime);
	if (hitShield) {
		inShieldTime += deltaTime;
		if (inShieldTime >= hitShieldTime) {
			inShieldTime = 0.f;
			particle->SetVelocity(glm::vec3(0.f));
			particle->SetPosition(glm::vec3(0.f, -10000.f, 0.f));
			hitShield = false;
		}
	}
	if (mesh->positionXYZ.y < LOWER_LIMIT && particle->GetVelocity().x > 0.f) {
		particle->SetVelocity(glm::vec3(0.f));
		particle->SetPosition(glm::vec3(0.f, -10000.f, 0.f));
	}
}

void cAlienMissile::StartMoving(glm::vec3 position) {
	particle->SetVelocity(glm::vec3(0.f, -500.f, 0.f));
	particle->SetPosition(position += glm::vec3(0.f,-50.f,0.f));
}

bool cAlienMissile::RecieveMessage(sMessage theMessage) {
	if (theMessage.command == "Destroy" && !hitShield) {
		hitShield = true;
	}
	return true;
}
bool cAlienMissile::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	return true;
}
bool cAlienMissile::SetReciever(iMediator* pTheReciever) {
	return true;
}