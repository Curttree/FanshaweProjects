#pragma once
#include <physics/cParticleWorld.h>
#include <physics/particle_force_generators.h>

#include "cMathHelper.h"

//Singleton implementation of world space.
//TODO: Determine if I need additional cleanup for pointers.
//TODO: Determin if gravity generator or world should be made private.
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
		//TODO: Determine if there is a better place for this.
		// Returns a vector laying on the x-z plane, randomized in direction and magnitude.
		// The output is designed to be linearly independent from the output of getRandomZVector()
		static glm::vec3 getRandomXVector();

		// Returns a vector laying on the x-z plane, randomized in direction and magnitude.
		// The output is designed to be linearly independent from the output of getRandomXVector()
		static glm::vec3 getRandomZVector();
};