#include "cFancyFirework1Object.h"
#include "cFireworkDebrisObject.h"

std::vector<cFireworkObject*> cFancyFirework1Object::triggerStageTwo() {
	std::vector<cFireworkObject*> newFireworks;
	for (int x = 0; x < 2; x++) {
		cFirework* firework = new cFireworkDebris(1.0f, particle->GetPosition());
		firework->SetVelocity(glm::vec3(float(x), 0.f, 0.f));
		worldSpace->_world->GetForceRegistry()->Register(firework, worldSpace->_gravityGenerator);
		cModel* newModel = new cModel();
		newModel->modelName = "assets/pokeball.ply";
		newModel->bOverriveVertexColourHACK = true;
		newModel->bIsWireframe = false;
		newModel->positionXYZ = particle->GetPosition();
		cFireworkObject* obj = new cFireworkDebrisObject(firework, newModel);
		newFireworks.push_back(obj);
		if (worldSpace->_world->AddParticle(obj->particle))
		{
			std::cout << "Hurray!" << std::endl;
		}
	}
	return newFireworks;
}