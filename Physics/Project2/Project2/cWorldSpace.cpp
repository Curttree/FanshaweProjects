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

		//Initialize physics
		_instance->_gravityGenerator = new nPhysics::cParticleGravityGenerator(glm::vec3(0.0f, -9.81f, 0.0f));
		_instance->_world = new nPhysics::cParticleWorld();
	}

	return _instance;
}

// MATH/Starter code from first physics assignment

glm::vec3 cWorldSpace::getRandomXVector()
{
	return glm::vec3(_mathHelper->getRandom(0.1f, 1.f), 0.f, _mathHelper->getRandom(0.1f, 1.f));
}

glm::vec3 cWorldSpace::getRandomZVector()
{
	return glm::vec3(-_mathHelper->getRandom(0.1f, 1.f), 0.f, _mathHelper->getRandom(0.1f, 1.f));
}

// If the user decides to override the alpha value, use the provided value rather than randomly assigning.
glm::vec3 cWorldSpace::getRandomVector3(float magnitude, bool overrideAlpha, float alpha)
{
	if (!overrideAlpha){
		alpha = _mathHelper->getRandom(glm::pi<float>());
	}
	float sinAlpha = glm::sin(alpha);
	float cosAlpha = glm::cos(alpha);

	float delta = _mathHelper->getRandom(-glm::pi<float>(), glm::pi<float>());
	float sinDelta = glm::sin(delta);
	float cosDelta = glm::cos(delta);

	return glm::vec3(magnitude * sinAlpha * cosDelta, magnitude * sinAlpha * sinDelta, magnitude * cosAlpha);
}

// Generate an orthonormal basis, given two linearly independent vectors.
// The input vectors are unaffected.
glm::mat3 cWorldSpace::orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec)
{
	// If randomized orthonormal basis complicates simulation in the future, replace with standard axis.
	glm::vec3 _xVec = glm::vec3(xVec);
	glm::vec3 _yVec = glm::cross(zVec, _xVec);
	glm::vec3 _zVec = glm::cross(_xVec, _yVec);

	// We are orthogonal, now normalize!
	_xVec = glm::normalize(_xVec);
	_yVec = glm::normalize(_yVec);
	_zVec = glm::normalize(_zVec);
	return glm::mat3(_xVec, _yVec, _zVec);
}