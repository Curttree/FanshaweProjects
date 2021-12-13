#include "cDungeonFactory.h"

#define DUNGEON_FLOOR 1
#define DUNGEON_WALL 2
#define DUNGEON_DOOR_FRAME 3
#define DUNGEON_PORTCULLIS_FRAME 4
#define DUNGEON_DOOR 5
#define DUNGEON_PORTCULLIS 6


cDungeonFactory* cDungeonFactory::_instance = 0;
cDungeonFactory::cDungeonFactory() {

}

cDungeonFactory* cDungeonFactory::Instance() {
	if (_instance == 0) {
		_instance = new cDungeonFactory;
	}

	return _instance;
}

cMesh* cDungeonFactory::createDungeonPiece(int type, glm::vec3 position, glm::vec3 orientation) {
	cMesh* obj;
	switch (type) {
	case(DUNGEON_FLOOR):
		obj = new cMesh();
		obj->meshName = "Floors/SM_Env_Dwarf_Floor_03.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->friendlyName = "Floor";
		obj->scale = 1.f;
		break;
	case(DUNGEON_WALL):
		obj = new cMesh();
		obj->meshName = "Walls/SM_Env_Dwarf_Wall_01.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Wall";
		obj->scale = 1.f;
		break;
	case(DUNGEON_DOOR_FRAME):
		obj = new cMesh();
		obj->meshName = "Doors/SM_Env_Dwarf_Wall_DoorFrame_Single_01.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "DoorFrame";
		obj->scale = 1.f;
		break;
	case(DUNGEON_PORTCULLIS_FRAME):
		obj = new cMesh();
		obj->meshName = "Doors/SM_Env_Dwarf_Wall_DoorFrame_Slider_01.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "PortcullisFrame";
		obj->scale = 1.f;
		break;
	case(DUNGEON_DOOR):
		obj = new cMesh();
		obj->meshName = "Doors/SM_Env_Dwarf_Wall_DoorFrame_Single_01_Door.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "DoorFrame";
		obj->scale = 1.f;
		break;
	case(DUNGEON_PORTCULLIS):
		obj = new cMesh();
		obj->meshName = "Doors/SM_Env_Dwarf_Wall_DoorFrame_Slider_01_Door.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "PortcullisFrame";
		obj->scale = 1.f;
		break;

	default:
		obj = NULL;
		break;
	}

	if (obj) {
		obj->positionXYZ = position;
	}
	return obj;
}
