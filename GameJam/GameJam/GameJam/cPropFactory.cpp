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

cProp* cPropFactory::createProp(int type, glm::vec3 position, glm::vec3 orientation, glm::vec3 velocity) {
	cProp* result = 0;
	switch (type) {
	case(PROP_CAN):
		result = ::g_pGameEngine->entityManager.CreateProp("can.ply", "BrainNerve.bmp", position, eShapeType::Cylinder,glm::vec3(0.5f),glm::vec3(1.5f,1.f,1.5f),1.f, orientation);
		result->mesh->lowDetailMeshName = "";
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_BOX):
		result = ::g_pGameEngine->entityManager.CreateProp("SM_Prop_CarboardBox_02.ply", "PolygonCity_Texture2.bmp", position, eShapeType::Box, glm::vec3(5.f), glm::vec3(1.f,1.f,1.f),50.f, orientation);
		result->mesh->lowDetailMeshName = "";
		result->positionOffset = glm::vec3(0.f, 0.3f, 0.f);
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_CITY_IMPOSTER):
		result = ::g_pGameEngine->entityManager.CreateProp("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply", "city_imposter.bmp", position, eShapeType::None, glm::vec3(160.f,50.f,10.f));
		result->mesh->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
		result->mesh->textureNames[8] = "city_imposter.bmp";
		result->mesh->bUseDiscardTransparency = true;
		result->mesh->meshName = "";
		result->mesh->highDetailMeshName = "";
		result->mesh->bDontLight = true;
		result->rotation = result->mesh->orientationXYZ;
		result->mesh->boundingRadius = 10.f;
		break;	
	case(PROP_TEST_GROUND):
		result = ::g_pGameEngine->entityManager.CreateProp("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply", "Concrete_012.bmp", position, eShapeType::Plane, glm::vec3(100.f));
		result->mesh->orientationXYZ = glm::vec3(-glm::pi<float>()/2.f, 0.f, 0.f);
		result->rotation = result->mesh->orientationXYZ;
		result->mesh->boundingRadius = 10.f;
		break;	
	case(PROP_HOTDOG_STAND):
		result = ::g_pGameEngine->entityManager.CreateProp("City/SM_Prop_HotdogStand_01.fbx", "PolygonCity_Texture2.bmp", position, eShapeType::Box, glm::vec3(2.5f), glm::vec3(1.5f, 1.4f, 1.5f), 500.f, orientation);
		result->mesh->lowDetailMeshName = "";
		result->positionOffset = glm::vec3(0.f, -1.f, 0.f);
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_BARRIER):
		result = ::g_pGameEngine->entityManager.CreateProp("City/SM_Prop_Barrier_01.fbx", "PolygonCity_Texture2.bmp", position, eShapeType::None, glm::vec3(4.f), glm::vec3(1.5f, -3.6f, 1.5f), 5.f, orientation);
		result->mesh->lowDetailMeshName = "";
		result->positionOffset = glm::vec3(0.f, -0.5f, 0.f);
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_BULLET):
		result = ::g_pGameEngine->entityManager.CreateProp("billiardball.ply", "15.bmp", position, eShapeType::Sphere,glm::vec3(0.5f),glm::vec3(0.5f),0.2f,glm::vec3(0.f),::g_pFlyCamera->getAtDirection()*30.f);
		break;
	case(PROP_PLANTER):
		result = ::g_pGameEngine->entityManager.CreateProp("City/SM_Bld_Apartment_Stairs_Planter_01.fbx", "PolygonCity_Texture2.bmp", position, eShapeType::Box, glm::vec3(1.f), glm::vec3(1.0f, 2.f, 2.f), 50000.f, orientation);
		result->mesh->lowDetailMeshName = "";
		result->positionOffset = glm::vec3(0.f, -0.f, -2.f);
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_LIGHT):
		result = ::g_pGameEngine->entityManager.CreateProp("City/SM_Prop_LightPole_Base_01.fbx", "PolygonCity_Texture2.bmp", position, eShapeType::Cylinder, glm::vec3(3.0f), glm::vec3(1.f, 5.f, 1.f), 50000.f, orientation);
		result->mesh->lowDetailMeshName = "";
		result->positionOffset = glm::vec3(0.f, -5.0f, 0.f);
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_PARKING):
		result = ::g_pGameEngine->entityManager.CreateProp("City/SM_Prop_ParkingMeter_01.fbx", "PolygonCity_Texture2.bmp", position, eShapeType::Cylinder, glm::vec3(3.0f), glm::vec3(0.5f, 4.f, 0.5f), 50000.f, orientation);
		result->mesh->lowDetailMeshName = "";
		result->positionOffset = glm::vec3(0.f, -4.f, 0.f);
		result->mesh->boundingRadius = 1.f;
		break;
	case(PROP_BENCH):
		result = ::g_pGameEngine->entityManager.CreateProp("City/SM_Prop_ParkBench_01.fbx", "PolygonCity_Texture2.bmp", position, eShapeType::Box, glm::vec3(3.f), glm::vec3(3.0f, 0.5f, 1.f), 50000.f, orientation);
		result->mesh->lowDetailMeshName = "";
		result->positionOffset = glm::vec3(0.f, -0.5f, -0.f);
		result->mesh->boundingRadius = 1.f;
		break;
	default:
		break;
	}
	return result;
}