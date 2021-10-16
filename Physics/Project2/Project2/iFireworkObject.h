#pragma once
#include "cFirework.h"
#include "src/graphics/cModel.h"

struct iFireworkObject {
public:
	virtual ~iFireworkObject();
	iFireworkObject();
	iFireworkObject(cFirework* _particle, cModel* _model);
	cFirework* particle;
	cModel* model; 
	virtual std::vector<iFireworkObject*> triggerStageTwo()=0;
	virtual bool isReadyForStageTwo()=0;
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
};