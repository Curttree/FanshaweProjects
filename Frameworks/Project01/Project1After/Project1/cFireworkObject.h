#pragma once
#include "cFirework.h"
#include "iFuse.h"
#include "iMessage.h"
#include "src/graphics/cModel.h"

class cFirework_Implementation;		//Forward declaration of firework implementation class (for PIMPL)

class cFireworkObject : public iMessage {
public:
	virtual ~cFireworkObject();
	cFireworkObject();
	cFireworkObject(cFirework* _particle, cModel* _model);
	cFirework* particle = 0;
	cModel* model = 0; 
	virtual std::vector<cFireworkObject*> triggerNextStage()=0;
	iFuse* fuse = 0;
	virtual void changeColour(glm::vec3 colour);
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
	cFirework_Implementation* implementation;
	sMessage outgoingMessage;
};