#include "cEntity.h"

cEntity::cEntity(cMesh* _mesh) {
	mesh = _mesh;
	glm::vec3 position = glm::vec3(mesh->positionXYZ.x, mesh->positionXYZ.y+particleScale, mesh->positionXYZ.z);
	glm::vec3 velocity = worldSpace->getRandomVector3(100.f);
	particle = new cParticle(position,100.f, particleScale);
	particle->SetVelocity(glm::vec3(velocity.x,0.f,velocity.z));
	worldSpace->_world->AddParticle(particle);
	worldSpace->_world->GetForceRegistry()->Register(particle, worldSpace->_gravityGenerator);
	debugMesh = new cMesh();
	debugMesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
	debugMesh->bIsWireframe = true;
	debugMesh->bUseObjectDebugColour = true;
	debugMesh->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	debugMesh->bDontLight = true;
	debugMesh->scale = particleScale;
	debugMesh->positionXYZ = mesh->positionXYZ;
}

cEntity::~cEntity() {
	//Cleanup particle
	//Cleanup particle mesh.
}

void cEntity::Update(float deltaTime) {
	//if (particle->GetPosition().y > particleScale) {
	//	particle->SetPosition(glm::vec3(particle->GetPosition().x, particleScale, particle->GetPosition().z));
	//}
	if (glm::length(particle->GetVelocity()) < 50.f) {
		glm::vec3 velocity = worldSpace->getRandomVector3(100.f);
		particle->SetVelocity(glm::vec3(velocity.x, 0.f, velocity.z));
	}
	mesh->positionXYZ = particle->GetPosition();
	mesh->positionXYZ.y -= particleScale;
	//mesh->orientationXYZ = worldSpace->getPositionInWorldSpace(particle->GetVelocity(), glm::vec3(0.f,0.f,-1.f));
	debugMesh->positionXYZ = particle->GetPosition();
}