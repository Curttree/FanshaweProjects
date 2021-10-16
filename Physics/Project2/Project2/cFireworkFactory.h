#pragma once
#include "iFireworkObject.h"

class cFireworkFactory {
public:
	static cFireworkFactory* Instance();
	iFireworkObject* createFireworkObject(int type, glm::vec3 position, glm::vec3 velocity);
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
private:
	cFireworkFactory();
	static cFireworkFactory* _instance;
	bool buildSharedFireworkDetails(iFireworkObject*& obj);
};