#pragma once
#include <physics/cParticleWorld.h>
#include <physics/particle_force_generators.h>

#include "cMathHelper.h"

class cWorldSpace {
	public:
		static cWorldSpace* Instance();
		nPhysics::cParticleGravityGenerator* _gravityGenerator;
		nPhysics::cParticleWorld* _world; 
		static glm::mat3 axes;
	private:
		cWorldSpace();

		static cWorldSpace* _instance;
		static cMathHelper* _mathHelper;

		static glm::mat3 orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec);
		static glm::vec3 getRandomXVector();
		static glm::vec3 getRandomZVector();
};