#include "cWorldSpace.h"


cWorldSpace::cWorldSpace()
{
}

cWorldSpace* cWorldSpace::_instance = 0;
glm::mat3  cWorldSpace::axes = orthonormalBasis(getRandomXVector(), getRandomZVector());
cMathHelper* cWorldSpace::_mathHelper = cMathHelper::Instance();
cWorldSpace* cWorldSpace::Instance() {
	if (_instance == 0) {
		_instance = new cWorldSpace;

		//Initialize Helpers
		//TODO: Double check if this is the correct approach, or if there is something more efficient.

		//Initialize physics
		_instance->_gravityGenerator = new nPhysics::cParticleGravityGenerator(glm::vec3(0.0f, -9.81f, 0.0f));
		_instance->_world = new nPhysics::cParticleWorld();
	}

	return _instance;
}

// MATH/Starter code from first physics assignment
// TODO: Rewrite my own version.

glm::vec3 cWorldSpace::getRandomXVector()
{
	return glm::vec3(_mathHelper->getRandom(0.1f, 1.f), 0.f, _mathHelper->getRandom(0.1f, 1.f));
}

glm::vec3 cWorldSpace::getRandomZVector()
{
	return glm::vec3(-_mathHelper->getRandom(0.1f, 1.f), 0.f, _mathHelper->getRandom(0.1f, 1.f));
}

// Generate an orthonormal basis, given two linearly independent vectors.
// The input vectors are unaffected.
glm::mat3 cWorldSpace::orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec)
{
	//TODO: Should this be a standard axis rather than randomly determined?

	glm::vec3 _xVec = glm::vec3(xVec);
	glm::vec3 _yVec = glm::cross(zVec, _xVec);
	glm::vec3 _zVec = glm::cross(_xVec, _yVec);

	// We are orthogonal, now normalize!
	_xVec = glm::normalize(_xVec);
	_yVec = glm::normalize(_yVec);
	_zVec = glm::normalize(_zVec);
	return glm::mat3(_xVec, _yVec, _zVec);
}