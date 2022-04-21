#pragma once
#include "cProp.h"
class cPropFactory {
public:
	static cPropFactory* Instance();
	cProp* createProp(int type, glm::vec3 position, glm::vec3 orientation = glm::vec3(0.f), glm::vec3 velocity = glm::vec3(0.f));
private:
	cPropFactory();
	static cPropFactory* _instance;
};