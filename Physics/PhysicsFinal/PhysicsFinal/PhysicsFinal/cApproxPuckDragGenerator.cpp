#include "cApproxPuckDragGenerator.h"

cApproxPuckDragGenerator::cApproxPuckDragGenerator(const glm::vec3& _acceleration)
{
	approxPuckDragAcceleration = _acceleration;
}

cApproxPuckDragGenerator::~cApproxPuckDragGenerator()
{

}

void cApproxPuckDragGenerator::Update(cParticle* particle, float deltaTime)
{
	particle->ApplyForce(approxPuckDragAcceleration * particle->GetMass());
}