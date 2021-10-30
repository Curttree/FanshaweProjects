#pragma once
#include "iFuse.h"
#include "cFireworkObject.h"

class cFireworkBuilder
{
	public:
		static cFireworkBuilder* Instance();
		cFireworkObject* buildFirework(int fireworkNum, glm::vec3 position, glm::vec3 debrisDirection = glm::vec3(0.f), glm::vec3 debrisColour = glm::vec3(1.f));
	protected:
		cWorldSpace* worldSpace = cWorldSpace::Instance();
		cMathHelper* mathHelper = cMathHelper::Instance();
	private:
		cFireworkBuilder();
		static cFireworkBuilder* _instance;
		glm::vec3 determineDirection(float lowerBound, float upperBound);
};