#pragma once

#include "iFireworkObject.h"

class cFireworkDebrisObject : public iFireworkObject {
public:
	cFireworkDebrisObject(cFirework* _particle, cModel* _model, glm::vec3 _colour);
	cFireworkDebrisObject();
	cFireworkDebrisObject(cFirework* _particle, cModel* _model);
	virtual std::vector<iFireworkObject*> triggerNextStage();
private:
	float _counter = 0;
};