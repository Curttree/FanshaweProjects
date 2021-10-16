#include "cFireworkFactory.h"

#include "cFancyFirework1Object.h"
#include "cFancyFirework2Object.h"
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

iFireworkObject* cFireworkFactory::createFireworkObject(int type, glm::vec3 position, glm::vec3 velocity) {
	cFirework* firework = new cFirework(1.0f, position);
	firework->SetVelocity(velocity);
	cModel* newModel = new cModel();
	newModel->positionXYZ = position;
	iFireworkObject* obj = 0;
	switch (type) {
	case(1):
		obj = new cFancyFirework1Object(firework, newModel);
		break;
	case(2):
		obj = new cFancyFirework2Object(firework, newModel);
		break;
	default:
		obj = new cFireworkDebrisObject(firework, newModel);
		break;
	}
	// Check to make sure obj has been set in case things have went horribly wrong.
	if (obj != 0) {
		buildSharedFireworkDetails(obj);
	}
	return obj;
}

bool cFireworkFactory::buildSharedFireworkDetails(iFireworkObject*& obj) {
	worldSpace->_world->AddParticle(obj->particle);
	worldSpace->_world->GetForceRegistry()->Register(obj->particle, worldSpace->_gravityGenerator);
	obj->model->modelName = "assets/pokeball.ply";
	obj->model->bOverriveVertexColourHACK = true;
	obj->model->bIsWireframe = false; 

	//Return value in case we want to verify values are properly set.
	// TODO: I may want to set to void.
	return true;
}