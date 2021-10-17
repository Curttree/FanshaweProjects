#pragma once
#include "cFirework.h"
#include "iFuse.h"
#include "src/graphics/cModel.h"

struct iFireworkObject {
public:
	virtual ~iFireworkObject();
	iFireworkObject();
	iFireworkObject(cFirework* _particle, cModel* _model);
	cFirework* particle = 0;
	cModel* model = 0; 
	virtual std::vector<iFireworkObject*> triggerStageTwo()=0;
	iFuse* fuse = 0;
	virtual void changeColour(glm::vec3 colour);
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
};