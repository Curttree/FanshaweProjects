#include "cAIPlayer.h"

cAIPlayer::cAIPlayer(cMesh* _mesh, float _debugScale, float _density) : cEntity(_mesh, _debugScale, _density) {
	glm::vec3 velocity = worldSpace->getRandomVector3(100.f);
	particle->SetVelocity(glm::vec3(velocity.x, 0.f, velocity.z));
	particle->SetDamping(1.f);
	debugMesh->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
void cAIPlayer::Update(float deltaTime) {
	velocity = particle->GetVelocity();
	if (glm::length(velocity) < 50.f) {
		velocity = worldSpace->getRandomVector3(100.f);
		particle->SetVelocity(glm::vec3(velocity.x, 0.f, velocity.z));
	}
	cEntity::Update(deltaTime);
}