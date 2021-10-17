#pragma once
#include "iFuse.h"
#include "iFireworkObject.h"

class cFireworkBuilder
{
	public:
		static cFireworkBuilder* Instance();
		iFireworkObject* buildFirework(int fireworkNum, glm::vec3 position, glm::vec3 velocity); 
	private:
		cFireworkBuilder();
		static cFireworkBuilder* _instance;
};