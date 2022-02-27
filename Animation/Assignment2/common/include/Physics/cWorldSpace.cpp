#include "cWorldSpace.h"
#include <iostream>

cWorldSpace::cWorldSpace()
{
}

// Particle on Particle collisions
cParticleParticleContactGenerator particleCollisionGenerator;


cWorldSpace* cWorldSpace::_instance = 0;
glm::mat3  cWorldSpace::axes = orthonormalBasis(getRandomXVector(), getRandomZVector());
cMathHelper* cWorldSpace::_mathHelper = cMathHelper::Instance();

cWorldSpace* cWorldSpace::Instance() {
	if (_instance == 0) {
		_instance = new cWorldSpace;

		//Initialize physics
		_instance->_gravityGenerator = new cGravityGenerator(glm::vec3(0.0f, -9.81f, 0.0f));
		_instance->_world = new cParticleWorld(500);
		_instance->_world->AddContactContactGenerator(&particleCollisionGenerator);
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

glm::vec3 cWorldSpace::getPositionInWorldSpace(const glm::vec3 orientationXYZ, const glm::vec3 startPositionXYZ) {
	glm::mat4 identity = glm::mat4(1.f);
	glm::mat4 result = identity;

	glm::mat4 rotateZ = glm::rotate(identity, orientationXYZ.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 rotateY = glm::rotate(identity, orientationXYZ.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotateX = glm::rotate(identity, orientationXYZ.x, glm::vec3(1.0f, 0.0f, 0.0f));

	result *= rotateZ;
	result *= rotateY;
	result *= rotateX;

	return result * glm::vec4(startPositionXYZ.x, startPositionXYZ.y, startPositionXYZ.z, 1.f);
}

void cWorldSpace::SetWorldBounds(glm::vec3 positiveBounds, glm::vec3 negativeBounds) {
	// Planes
	cPlaneParticleContactGenerator* groundGenerator = new cPlaneParticleContactGenerator(glm::vec3(0.f, 1.f, 0.f), negativeBounds.y);
	cPlaneParticleContactGenerator* ceilingGenerator = new cPlaneParticleContactGenerator(glm::vec3(0.f, -1.f, 0.f), -positiveBounds.y);
	cPlaneParticleContactGenerator* wallGenerator = new cPlaneParticleContactGenerator(glm::vec3(1.f, 0.f, 0.f), negativeBounds.x);
	cPlaneParticleContactGenerator* wall2Generator = new cPlaneParticleContactGenerator(glm::vec3(-1.f, 0.f, 0.f), -positiveBounds.x);
	cPlaneParticleContactGenerator* backWallGenerator = new cPlaneParticleContactGenerator(glm::vec3(0.f, 0.f, -1.f), -positiveBounds.z);
	cPlaneParticleContactGenerator* frontWallGenerator = new cPlaneParticleContactGenerator(glm::vec3(0.f, 0.f, 1.f), negativeBounds.z);
	_instance->_world->AddContactContactGenerator(groundGenerator);
	_instance->_world->AddContactContactGenerator(ceilingGenerator);
	_instance->_world->AddContactContactGenerator(wallGenerator);
	_instance->_world->AddContactContactGenerator(wall2Generator);
	_instance->_world->AddContactContactGenerator(backWallGenerator);
	_instance->_world->AddContactContactGenerator(frontWallGenerator);
	std::cout << "World bounds set" << std::endl;

}