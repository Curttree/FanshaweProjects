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

iFireworkObject* cFireworkFactory::createFireworkObject(int type, glm::vec3 position, glm::vec3 velocity, glm::vec3 debrisColour) {
	cFirework* firework = new cFirework(1.0f, position);
	firework->SetVelocity(velocity);
	cModel* newModel = new cModel();
	newModel->positionXYZ = position;
	iFireworkObject* obj = 0;
	switch (type) {
	case(1):
		obj = new cFancyFirework1Object(firework, newModel);
		obj->model->vertexColourOverrideHACK = glm::vec3(1.f, 0.f, 0.f);
		break;
	case(2):
		obj = new cFancyFirework2Object(firework, newModel);
		obj->model->vertexColourOverrideHACK = glm::vec3(0.f, 1.f, 0.f);
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

void cFireworkFactory::buildSharedFireworkDetails(iFireworkObject*& obj) {
	worldSpace->_world->AddParticle(obj->particle);
	worldSpace->_world->GetForceRegistry()->Register(obj->particle, worldSpace->_gravityGenerator);
	obj->model->modelName = "assets/pokeball.ply";
	obj->model->bOverriveVertexColourHACK = true;
	obj->model->bIsWireframe = false; 
}