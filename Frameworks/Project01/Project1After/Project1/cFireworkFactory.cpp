#include "cFireworkFactory.h"

#include "cFancyFirework1Object.h"
#include "cFancyFirework2Object.h"
#include "cFancyFirework3Object.h"
#include "cFancyFirework4Object.h"
#include "cFancyFirework5Object.h"
#include "cFireworkDebrisObject.h"

cFireworkFactory* cFireworkFactory::_instance = 0;
cFireworkFactory::cFireworkFactory() {

}

cFireworkFactory* cFireworkFactory::Instance() {
	if (_instance == 0) {
		_instance = new cFireworkFactory;
	}

	return _instance;
}

cFireworkObject* cFireworkFactory::createFireworkObject(int type, glm::vec3 position, glm::vec3 velocity, glm::vec3 debrisColour) {
	cFirework* firework = new cFirework(1.0f, position);
	firework->SetVelocity(velocity);
	cModel* newModel = new cModel();
	newModel->positionXYZ = position;
	cFireworkObject* obj = 0;
	switch (type) {
	case(1):
		obj = new cFancyFirework1Object(firework, newModel);
		break;
	case(2):
		obj = new cFancyFirework2Object(firework, newModel);
		break;
	case(3):
		obj = new cFancyFirework3Object(firework, newModel);
		break;	
	case(4):
		obj = new cFancyFirework4Object(firework, newModel);
		break;
	case(5):
		obj = new cFancyFirework5Object(firework, newModel);
		break;
	default:
		obj = new cFireworkDebrisObject(firework, newModel, debrisColour);
		break;
	}
	// Check to make sure obj has been set in case things have went horribly wrong.
	if (obj != 0) {
		buildSharedFireworkDetails(obj);
	}
	return obj;
}

void cFireworkFactory::buildSharedFireworkDetails(cFireworkObject*& obj) {
	worldSpace->_world->AddParticle(obj->particle);
	worldSpace->_world->GetForceRegistry()->Register(obj->particle, worldSpace->_gravityGenerator);
	obj->model->modelName = "assets/pokeball.ply";
	obj->model->bOverriveVertexColourHACK = true;
	obj->model->bIsWireframe = false;
	obj->model->bDontLightObject = true;
}