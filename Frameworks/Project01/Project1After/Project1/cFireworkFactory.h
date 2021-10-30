#pragma once
#include "cFireworkObject.h"

class cFireworkFactory {
public:
	static cFireworkFactory* Instance();
	cFireworkObject* createFireworkObject(int type, glm::vec3 position, glm::vec3 velocity, glm::vec3 debrisColour = glm::vec3(0.f));
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
private:
	cFireworkFactory();
	static cFireworkFactory* _instance;
	void buildSharedFireworkDetails(cFireworkObject*& obj);
};