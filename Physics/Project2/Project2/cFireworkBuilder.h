#pragma once
#include "iFuse.h"
#include "iFireworkObject.h"

class cFireworkBuilder
{
	public:
		static cFireworkBuilder* Instance();
		iFireworkObject* buildFirework(int fireworkNum, glm::vec3 position, glm::vec3 velocity, glm::vec3 debrisColour = glm::vec3(1.f));
	private:
		cFireworkBuilder();
		static cFireworkBuilder* _instance;
};