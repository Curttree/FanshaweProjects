#pragma once
#include "cFirework.h"
#include "cFireworkDebris.h"
#include "src/graphics/cModel.h"

struct cFireworkObject {
public:
	virtual ~cFireworkObject();
	cFireworkObject();
	cFireworkObject(cFirework* _particle, cModel* _model);
	cFirework* particle;
	cModel* model; 
	virtual std::vector<cFireworkObject*> triggerStageTwo()=0;
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
};