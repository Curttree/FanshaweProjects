#pragma once
#include "cFirework.h"
#include "iFuse.h"
#include "src/graphics/cModel.h"

struct iFireworkObject {
public:
	virtual ~iFireworkObject();
	iFireworkObject();
	iFireworkObject(cFirework* _particle, cModel* _model);
	cFirework* particle;
	cModel* model; 
	virtual std::vector<iFireworkObject*> triggerStageTwo()=0;
	iFuse* fuse;
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
};