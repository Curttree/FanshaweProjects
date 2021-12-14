#include "cDungeonFactory.h"

#define DUNGEON_FLOOR 1
#define DUNGEON_WALL 2
#define DUNGEON_DOOR_FRAME 3
#define DUNGEON_PORTCULLIS_FRAME 4
#define DUNGEON_DOOR 5
#define DUNGEON_PORTCULLIS 6


#define CRYSTAL_1 7
#define CRYSTAL_2 8
#define CRYSTAL_3 9
#define CRYSTAL_4 10

#define TREE_1 11
#define TREE_2 12
#define TREE_3 13

#define PLANT_1 14
#define PLANT_2 15
#define PLANT_3 16
#define PLANT_4 17
#define PLANT_5 18
#define PLANT_6 19

#define TORCH 20
#define STAIRS 21


#define SUNBEAM 22


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
	case(CRYSTAL_1):
		obj = new cMesh();
		obj->meshName = "Crystals/SM_Env_Crystals_Cluster_Large_01.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Crystal";
		obj->scale = 1.f;
		obj->alphaTransparency = 0.86;
		obj->wholeObjectShininess_SpecPower = 0.00001f;
		break;
	case(CRYSTAL_2):
		obj = new cMesh();
		obj->meshName = "Crystals/SM_Env_Crystals_Cluster_Large_02.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Crystal";
		obj->scale = 1.f;
		obj->alphaTransparency = 0.7;
		obj->wholeObjectShininess_SpecPower = 0.000001f;
		break;
	case(CRYSTAL_3):
		obj = new cMesh();
		obj->meshName = "Crystals/SM_Env_Crystals_Cluster_Large_03.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Crystal";
		obj->scale = 1.f;
		obj->alphaTransparency = 0.9;
		obj->wholeObjectShininess_SpecPower = 0.00001f;
		break;
	case(CRYSTAL_4):
		obj = new cMesh();
		obj->meshName = "Crystals/SM_Env_Crystals_Cluster_Large_04.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Crystal";
		obj->scale = 1.f;
		obj->alphaTransparency = 0.8;
		obj->wholeObjectShininess_SpecPower = 0.00001f;
		break;
	case(TREE_1):
		obj = new cMesh();
		obj->meshName = "Plants/SM_Generic_Tree_02.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Tree1";
		obj->scale = 2.f;
		break;
	case(TREE_2):
		obj = new cMesh();
		obj->meshName = "Plants/SM_Generic_Tree_04.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Tree2";
		obj->scale = 2.f;
		break;
	case(TREE_3):
		obj = new cMesh();
		obj->meshName = "Plants/SM_Env_Tree_Dead_01.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Tree3";
		obj->scale = 40.f;
		break;
	case(PLANT_1):
		obj = new cMesh();
		obj->meshName = "Plants/SM_Env_Plant_01.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Plant";
		obj->scale = 1.f;
		break;
	case(PLANT_2):
		obj = new cMesh();
		obj->meshName = "Plants/SM_Env_Plant_02.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Plant";
		obj->scale = 1.f;
		break;
	case(PLANT_3):
		obj = new cMesh();
		obj->meshName = "Plants/SM_Env_Plant_Spikey_01.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Plant";
		obj->scale = 1.f;
		break;
	case(PLANT_4):
		obj = new cMesh();
		obj->meshName = "Plants/SM_Env_Plants_01.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Plant";
		obj->scale = 100.f;
		break;
	case(PLANT_5):
		obj = new cMesh();
		obj->meshName = "Plants/SM_Env_Plants_02.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Plant";
		obj->scale = 100.f;
		break;
	case(PLANT_6):
		obj = new cMesh();
		obj->meshName = "Plants/SM_Env_Plants_03.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Plant";
		obj->scale = 100.f;
		break;
	case(TORCH):
		obj = new cMesh();
		obj->meshName = "SM_Prop_Dwarf_Torch_05.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->positionXYZ.y = 400.f;
		obj->friendlyName = "Torch";
		obj->scale = 1.f;
		break;
	case(STAIRS):
		obj = new cMesh();
		obj->meshName = "SM_Env_Dwarf_Stairs_01.ply";
		obj->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Stairs";
		obj->scale = 1.f;
		break; 

	case(SUNBEAM):
		obj = new cMesh();
		obj->meshName = "FX_SunShafts.ply";
		obj->textureNames[0] = "spotlight.bmp";
		obj->textureNames[8] = "spotlight.bmp";
		obj->textureRatios[0] = 1.f;
		obj->orientationXYZ = orientation;
		obj->friendlyName = "Sunbeam";
		obj->scale = 600.f;
		obj->bUseDiscardTransparency = true;
		obj->bDontLight = false;
		obj->alphaTransparency = 0.05f;
		obj->discardColour = glm::vec3(0.f, 0.f, 0.f);
		break;
	default:
		obj = NULL;
		break;
	}

	if (obj) {
		obj->positionXYZ += position;
	}
	return obj;
}
