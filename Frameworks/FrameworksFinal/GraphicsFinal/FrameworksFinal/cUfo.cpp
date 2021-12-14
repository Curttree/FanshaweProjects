#include "cUfo.h"
#include "globals.h"

#define UPPER_LIMIT 1500.f

cMathHelper* _mathHelper = cMathHelper::Instance();

cUfo::cUfo() {
	mesh = new cMesh();
	mesh->bDontLight = true;
	mesh->bUseWholeObjectDiffuseColour = true;
	mesh->wholeObjectSpecularRGB = glm::vec3(1.f, 1.f, 1.f);
	mesh->wholeObjectDiffuseRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
	mesh->friendlyName = "UFO";
	mesh->scale = 1.f;
	mesh->meshName = "Invaders/SpaceInvader_UFO_block.ply";
	mesh->positionXYZ = glm::vec3(10000.f, 10000.f, 0.f);

	particleScale = 120.f;
	glm::vec3 position = glm::vec3(mesh->positionXYZ.x, mesh->positionXYZ.y + particleScale, mesh->positionXYZ.z);
	particle = new cParticle(position, 120.f, particleScale);
	particle->SetDamping(0.9f);
	particle->type = mesh->friendlyName;
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


void cUfo::Update(float deltaTime) {
	cEntity::Update(deltaTime);
	if (mesh->positionXYZ.y > UPPER_LIMIT) {
		particle->SetVelocity(glm::vec3(0.f));
		particle->SetPosition(glm::vec3(0.f, 10000.f, 0.f));
	}
}

void cUfo::StartMoving() {
	if (_mathHelper->getRandom(0, 2) < 1.f) {
		direction = 1;
	}
	else {
		direction = -1;
	}
	particle->SetVelocity(direction * glm::vec3(500.f, 0.f, 0.f));
	particle->SetPosition( glm::vec3(-1.f * direction * 1100.f, 1250.f, 0.f));
}