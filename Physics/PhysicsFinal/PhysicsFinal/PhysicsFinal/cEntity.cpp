#include "cEntity.h"
cEntity::cEntity(cMesh* _mesh, float _debugScale, float _density) {
	particleScale = _debugScale;
	mesh = _mesh;
	glm::vec3 position = glm::vec3(mesh->positionXYZ.x, mesh->positionXYZ.y+particleScale, mesh->positionXYZ.z);
	particle = new cParticle(position,_density, particleScale);
	particle->SetDamping(0.8f);
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

cEntity::~cEntity() {
	//Cleanup particle
	//Cleanup particle mesh.
}

void cEntity::Update(float deltaTime) {
	velocity = particle->GetVelocity();
	mesh->positionXYZ = particle->GetPosition();
	mesh->positionXYZ.y -= particleScale;
	mesh->orientationXYZ.y = atan2(velocity.x, velocity.z);
	debugMesh->positionXYZ = particle->GetPosition();
}

cMesh* cEntity::GetDebugMesh() {
	return debugMesh;
}