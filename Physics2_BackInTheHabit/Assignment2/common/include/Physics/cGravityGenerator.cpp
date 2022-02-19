#include "cGravityGenerator.h"
cGravityGenerator::cGravityGenerator(const glm::vec3& _gravityAcceleration)
{
	gravityAcceleration = _gravityAcceleration;
}

cGravityGenerator::~cGravityGenerator()
{

}

void cGravityGenerator::Update(cParticle* particle, float deltaTime)
{
	particle->ApplyForce(gravityAcceleration * particle->GetMass());
}