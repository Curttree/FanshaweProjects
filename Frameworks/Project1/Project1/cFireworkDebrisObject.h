#pragma once

#include "cFireworkObject.h"

class cFireworkDebrisObject : public cFireworkObject {
public:
	cFireworkDebrisObject(cFirework* _particle, cModel* _model, glm::vec3 _colour);
	cFireworkDebrisObject(cFirework* _particle, cModel* _model);
	virtual std::vector<cFireworkObject*> triggerNextStage();
private:
	float _counter = 0;
};