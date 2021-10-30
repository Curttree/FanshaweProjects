#include "cFireworkMediator.h"

bool cFireworkMediator::RecieveMessage(sMessage theMessage) {
	if (theMessage.command == "CREATE FIREWORK") {
		cFireworkObject* newObj = fireworkBuilder->buildFirework(theMessage.vec_iData[0], theMessage.vec_v4Data[0]);
		newObj->SetReciever(this);
		fireworkObjs.push_back(newObj);
		return true;
	}
	else if (theMessage.command == "UPDATE POSITIONS") {
		glm::vec3 position;
		for (size_t x = 0; x < fireworkObjs.size(); x++) {
			fireworkObjs[x]->particle->GetPosition(position);
			fireworkObjs[x]->model->positionXYZ = position;
		}
		return true;
	}
	else if (theMessage.command == "TIME STEP") {
		TimeStep(theMessage.vec_fData[0]);
	}
	return true;
}

bool cFireworkMediator::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	if (theMessage.command == "GET FIREWORKS") {
		theResponse.vec_fireworkData = fireworkObjs;
		return true;
	}

	return true;
}

bool cFireworkMediator::SetReciever(iMediator* pTheReciever) {
	// Doesn't do anything. This is the mediator.
	return true;
}

void cFireworkMediator::TimeStep(float deltaTime) {
	for (int x = 0; x < fireworkObjs.size(); x++)
	{
		if (fireworkObjs[x]->fuse->isReadyForNextStage()) {
			std::vector<cFireworkObject*> newFireworks = fireworkObjs[x]->triggerNextStage();
			if (newFireworks.size() > 0) {
				fireworkObjs.insert(fireworkObjs.end(), newFireworks.begin(), newFireworks.end());
			}
			worldSpace->_world->RemoveParticle(fireworkObjs[x]->particle);
			delete fireworkObjs[x];
			fireworkObjs[x] = 0;
			fireworkObjs.erase(fireworkObjs.begin() + x);
			// Decrement x. Size of vector shrunk, so index has decreased by 1.
			x--;
		}
	}
	worldSpace->_world->TimeStep(deltaTime);
}