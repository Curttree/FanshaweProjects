#include "DecorationManager.h"
#include "globals.h"

DecorationManager* DecorationManager::_instance = 0;
DecorationManager::DecorationManager() {

}

DecorationManager* DecorationManager::Instance() {
    if (_instance == 0) {
        _instance = new DecorationManager;
    }

    return _instance;
}
void DecorationManager::DecorateScene() {
    int startZ = -50;
    int endZ = 400;
    bool drawBuildings = true;
    std::string door, stack, roof;
    for (int currentZ = startZ; currentZ <endZ; currentZ += 10)
    {
        if (drawBuildings) {
            GetBuildingParts(door, stack, roof);
            cMesh* door1 = new cMesh();
            door1->setAllMeshNames(door);
            door1->lowDetailMeshName = "";
            door1->scale = glm::vec3(4.0f);
            door1->positionXYZ = glm::vec3(25.f, -3.f, (float)currentZ);
            door1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
            door1->textureNames[0] = "PolygonCity_Texture2.bmp";
            door1->textureRatios[0] = 1.f;
            ::g_vec_pMeshes.push_back(door1);
            cMesh* stack1 = new cMesh();
            stack1->setAllMeshNames(stack);
            stack1->lowDetailMeshName = "";
            stack1->scale = glm::vec3(4.0f);
            stack1->positionXYZ = glm::vec3(25.f, 7.f, (float)currentZ);
            stack1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
            stack1->textureNames[0] = "PolygonCity_Texture2.bmp";
            stack1->textureRatios[0] = 1.f;
            ::g_vec_pMeshes.push_back(stack1);
            cMesh * roof1 = new cMesh();
            roof1->setAllMeshNames(roof);
            roof1->lowDetailMeshName = "";
            roof1->scale = glm::vec3(4.0f);
            roof1->positionXYZ = glm::vec3(25.f, 47.f, (float)currentZ);
            roof1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
            roof1->textureNames[0] = "PolygonCity_Texture2.bmp";
            roof1->textureRatios[0] = 1.f;
            ::g_vec_pMeshes.push_back(roof1);

            GetBuildingParts(door, stack, roof);
            cMesh* door2 = new cMesh();
            door2->setAllMeshNames(door);
            door2->lowDetailMeshName = "";
            door2->scale = glm::vec3(4.0f);
            door2->positionXYZ = glm::vec3(-25.f, -3.f, (float)currentZ+20.f);
            door2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
            door2->textureNames[0] = "PolygonCity_Texture2.bmp";
            door2->textureRatios[0] = 1.f;
            ::g_vec_pMeshes.push_back(door2);
            cMesh* stack2 = new cMesh();
            stack2->setAllMeshNames(stack);
            stack2->lowDetailMeshName = "";
            stack2->scale = glm::vec3(4.0f);
            stack2->positionXYZ = glm::vec3(-25.f, 7.f, (float)currentZ + 20.f);
            stack2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
            stack2->textureNames[0] = "PolygonCity_Texture2.bmp";
            stack2->textureRatios[0] = 1.f;
            ::g_vec_pMeshes.push_back(stack2);
            cMesh* roof2 = new cMesh();
            roof2->setAllMeshNames(roof);
            roof2->lowDetailMeshName = "";
            roof2->scale = glm::vec3(4.0f);
            roof2->positionXYZ = glm::vec3(-25.f, 47.f, (float)currentZ + 20.f);
            roof2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
            roof2->textureNames[0] = "PolygonCity_Texture2.bmp";
            roof2->textureRatios[0] = 1.f;
            ::g_vec_pMeshes.push_back(roof2);
            drawBuildings = false;
        }
        else {
            drawBuildings = true;
        }
        cMesh* side1extra = new cMesh();
        side1extra->setAllMeshNames("City/SM_Env_Sidewalk_Straight_01.fbx");
        side1extra->lowDetailMeshName = "";
        side1extra->scale = glm::vec3(2.0f);
        side1extra->positionXYZ = glm::vec3(30.f, -3.f, (float)currentZ + 10.f);
        side1extra->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
        side1extra->textureNames[0] = "PolygonCity_Texture2.bmp";
        side1extra->textureRatios[0] = 1.f;
        ::g_vec_pMeshes.push_back(side1extra);
        cMesh* side1 = new cMesh();
        side1->setAllMeshNames("City/SM_Env_Sidewalk_Straight_01.fbx");
        side1->lowDetailMeshName = "";
        side1->scale = glm::vec3(2.0f);
        side1->positionXYZ = glm::vec3(10.f, -3.f, (float)currentZ);
        side1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
        side1->textureNames[0] = "PolygonCity_Texture2.bmp";
        side1->textureRatios[0] = 1.f;
        ::g_vec_pMeshes.push_back(side1);
        cMesh* road1 = new cMesh();
        road1->setAllMeshNames("City/SM_Env_Road_YellowLines_02.fbx");
        road1->lowDetailMeshName = "";
        road1->scale = glm::vec3(2.0f);
        road1->positionXYZ = glm::vec3(10.f, -3.f, (float)currentZ);
        road1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
        road1->textureNames[0] = "PolygonCity_Road_01.bmp";
        road1->textureRatios[0] = 1.f;
        ::g_vec_pMeshes.push_back(road1);
        cMesh* road2 = new cMesh();
        road2->setAllMeshNames("City/SM_Env_Road_YellowLines_02.fbx");
        road2->lowDetailMeshName = "";
        road2->scale = glm::vec3(2.0f);
        road2->positionXYZ = glm::vec3(-10.f, -3.f, (float)currentZ+10.f);
        road2->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        road2->textureNames[0] = "PolygonCity_Road_01.bmp";
        road2->textureRatios[0] = 1.f;
        ::g_vec_pMeshes.push_back(road2);
        cMesh* side2 = new cMesh();
        side2->setAllMeshNames("City/SM_Env_Sidewalk_Straight_01.fbx");
        side2->lowDetailMeshName = "";
        side2->scale = glm::vec3(2.0f);
        side2->positionXYZ = glm::vec3(-10.f, -3.f, (float)currentZ + 10.f);
        side2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
        side2->textureNames[0] = "PolygonCity_Texture2.bmp";
        side2->textureRatios[0] = 1.f;
        ::g_vec_pMeshes.push_back(side2);
        cMesh* side2extra = new cMesh();
        side2extra->setAllMeshNames("City/SM_Env_Sidewalk_Straight_01.fbx");
        side2extra->lowDetailMeshName = "";
        side2extra->scale = glm::vec3(2.0f);
        side2extra->positionXYZ = glm::vec3(-30.f, -3.f, (float)currentZ);
        side2extra->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
        side2extra->textureNames[0] = "PolygonCity_Texture2.bmp";
        side2extra->textureRatios[0] = 1.f;
        ::g_vec_pMeshes.push_back(side2extra);
    }

}

void DecorationManager::GetBuildingParts(std::string& door, std::string& stack, std::string& roof) {
    int doorNum = rand() % 2;
    switch (doorNum) {
    case 0:
        door = "City/SM_Bld_Apartment_Door_01.fbx";
        break;
    default:
        door = "City/SM_Bld_Apartment_Door_02.fbx";
        break;
    }
    int stackNum = rand() % 3;
    switch (stackNum) {
    case 0:
        stack = "City/SM_Bld_Apartment_Stack_01.fbx";
        break;
    case 1:
        stack = "City/SM_Bld_Apartment_Stack_02.fbx";
        break;
    default:
        stack = "City/SM_Bld_Apartment_Stack_03.fbx";
        break;
    }
    int roofNum = rand() % 3;
    switch (roofNum) {
    case 0:
        roof = "City/SM_Bld_Apartment_Roof_01.fbx";
        break;
    case 1:
        roof = "City/SM_Bld_Apartment_Roof_02.fbx";
        break;
    default:
        roof = "City/SM_Bld_Apartment_Roof_03.fbx";
        break;
    }
    return;
}