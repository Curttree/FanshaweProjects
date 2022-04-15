#include "cPropFactory.h"
#include "propDefs.h"
#include "globals.h"


cPropFactory* cPropFactory::_instance = 0;
cPropFactory::cPropFactory() {

}

cPropFactory* cPropFactory::Instance() {
	if (_instance == 0) {
		_instance = new cPropFactory;
	}

	return _instance;
}

cProp* cPropFactory::createProp(int type, glm::vec3 position, glm::vec3 velocity) {
	cProp* result = 0;
	switch (type) {
	case(PROP_CAN):
		result = ::g_pGameEngine->entityManager.CreateProp("can.ply", "BrainNerve.bmp", position);
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_BOX):
		result = ::g_pGameEngine->entityManager.CreateProp("SM_Prop_CarboardBox_01.ply", "PolygonCity_Texture.bmp", position, glm::vec3(10.f));
		result->mesh->lowDetailMeshName = "";
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_CITY_IMPOSTER):
		result = ::g_pGameEngine->entityManager.CreateProp("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply", "city_imposter.bmp", position, glm::vec3(10.f)); 
		result->mesh->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
		result->rotation = result->mesh->orientationXYZ;
		result->mesh->meshName = "";
		result->mesh->boundingRadius = 10.f;
		break;
	default:
		break;
	}
	return result;
}