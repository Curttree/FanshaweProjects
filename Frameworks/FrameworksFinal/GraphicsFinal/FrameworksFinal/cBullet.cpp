#include "cBullet.h"

#define UPPER_LIMIT 1500.f

cBullet::cBullet(){
	mesh = new cMesh();
	mesh->bDontLight = true;
	mesh->bUseWholeObjectDiffuseColour = true;
	mesh->wholeObjectSpecularRGB = glm::vec3(0.f, 1.f, 0.f);
	mesh->wholeObjectDiffuseRGBA = glm::vec4(0.f, 1.f, 0.f, 1.f);
	mesh->friendlyName = "Bullet";
	mesh->scale = 1.f;
	mesh->meshName = "Invaders/Invader_Single_Cube.ply";
	mesh->positionXYZ = glm::vec3(0.f, 9000.f, 0.f);

	particleScale = 1.f;
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
}


void cBullet::Update(float deltaTime) {
	cEntity::Update(deltaTime);
	if (mesh->positionXYZ.y > UPPER_LIMIT) {
		particle->SetVelocity(glm::vec3(0.f));
		particle->SetPosition(glm::vec3(0.f, 10000.f, 0.f));
	}
}

bool cBullet::RecieveMessage(sMessage theMessage) {
	if (theMessage.command == "Destroy") {
		particle->SetVelocity(glm::vec3(0.f));
		particle->SetPosition(glm::vec3(0.f, 10000.f, 0.f));
	}
	return true;
}
bool cBullet::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	return true;
}
bool cBullet::SetReciever(iMediator* pTheReciever) {
	return true;
}