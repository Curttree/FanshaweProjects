#pragma once
#include <extern/glm/gtc/matrix_transform.hpp>

#include "cMathHelper.h"
#include "cParticleWorld.h"
#include "cGravityGenerator.h"
#include "cPlaneParticleContactGenerator.h"
#include "cParticleParticleContactGenerator.h"

class cWorldSpace {
	public:
		static cWorldSpace* Instance();
		cGravityGenerator* _gravityGenerator;
		cParticleWorld* _world; 
		static glm::mat3 axes;
		static glm::vec3 getRandomVector3(float magnitude, bool overrideAlpha = false, float alpha = 0);
		static glm::vec3 getPositionInWorldSpace(const glm::vec3 orientationXYZ, const glm::vec3 startPositionXYZ);
	private:
		cWorldSpace();

		static cWorldSpace* _instance;
		static cMathHelper* _mathHelper;

		static glm::mat3 orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec);
		static glm::vec3 getRandomXVector();
		static glm::vec3 getRandomZVector();
};