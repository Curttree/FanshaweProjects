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
		result = ::g_pGameEngine->entityManager.CreateProp("can.ply", "BrainNerve.bmp", position, eShapeType::Cylinder,glm::vec3(0.5f));
		result->mesh->lowDetailMeshName = "";
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_BOX):
		result = ::g_pGameEngine->entityManager.CreateProp("SM_Prop_CarboardBox_02.ply", "PolygonCity_Texture.bmp", position, eShapeType::Box, glm::vec3(10.f), glm::vec3(2.5f,2.5f,2.5f),5.f);
		result->mesh->lowDetailMeshName = "";
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_CITY_IMPOSTER):
		result = ::g_pGameEngine->entityManager.CreateProp("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply", "city_imposter.bmp", position, eShapeType::None, glm::vec3(10.f));
		result->mesh->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
		result->rotation = result->mesh->orientationXYZ;
		result->mesh->meshName = "";
		result->mesh->boundingRadius = 10.f;
		break;	
	case(PROP_TEST_GROUND):
		result = ::g_pGameEngine->entityManager.CreateProp("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply", "Concrete_012.bmp", position, eShapeType::Plane, glm::vec3(100.f));
		result->mesh->orientationXYZ = glm::vec3(-glm::pi<float>()/2.f, 0.f, 0.f);
		result->rotation = result->mesh->orientationXYZ;
		result->mesh->boundingRadius = 10.f;
		break;	
	case(PROP_BUILDING_1):
		result = ::g_pGameEngine->entityManager.CreateProp("City/SM_Bld_Apartment_Door_01.fbx", "PolygonCity_Texture.bmp", position, eShapeType::Box, glm::vec3(10.f), glm::vec3(2.5f, 2.5f, 2.5f), 5.f);
		result->mesh->lowDetailMeshName = "";
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_BULLET):
		result = ::g_pGameEngine->entityManager.CreateProp("billiardball.ply", "15.bmp", position, eShapeType::Sphere,glm::vec3(0.5f),glm::vec3(0.5f),0.2f,glm::vec3(0.f),::g_pFlyCamera->getAtDirection()*30.f);
		break;
	default:
		break;
	}
	return result;
}