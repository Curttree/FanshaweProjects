#include "DecorationManager.h"
#include "globals.h"
#include "particleDefs.h"

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
    rightVents = { 0,50,100,150,200,250,300 };
    leftVents = { 20,70,120,170,220,270,320 };
    
    BuildStreet(-50,400); 
    BuildStreetsAhead(400.f);
    AddVents();
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

void DecorationManager::BuildStreetsAhead(float starting) {
    //This is pure brute force environment building. It will only be used to generate a sample of the rest of the building.
    //Should be cleaned up if running as part of execution.
    float boundingRadius = -1.f;
    {
        cMesh* side1 = new cMesh("City/SM_Env_Sidewalk_Straight_01.fbx");
        side1->lowDetailMeshName = "";
        side1->scale = glm::vec3(2.0f);
        side1->positionXYZ = glm::vec3(10.f, -3.f, starting - 10.f);
        side1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
        side1->textureNames[0] = "PolygonCity_Texture2.bmp";
        side1->textureRatios[0] = 1.f;
        side1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side1);
    }
    {
        cMesh* side1 = new cMesh("City/SM_Env_Sidewalk_Straight_01.fbx");
        side1->lowDetailMeshName = "";
        side1->scale = glm::vec3(2.0f);
        side1->positionXYZ = glm::vec3(10.f, -3.f, starting + 0.f);
        side1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
        side1->textureNames[0] = "PolygonCity_Texture2.bmp";
        side1->textureRatios[0] = 1.f;
        side1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side1);
    }
    {
        cMesh* road1 = new cMesh("City/SM_Env_Road_YellowLines_02.fbx");
        road1->lowDetailMeshName = "";
        road1->scale = glm::vec3(2.0f);
        road1->positionXYZ = glm::vec3(10.f, -3.f, starting + 0.f);
        road1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
        road1->textureNames[0] = "PolygonCity_Road_01.bmp";
        road1->textureRatios[0] = 1.f;
        road1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(road1);
        cMesh* road2 = new cMesh("City/SM_Env_Road_YellowLines_02.fbx");
        road2->lowDetailMeshName = "";
        road2->scale = glm::vec3(2.0f);
        road2->positionXYZ = glm::vec3(-10.f, -3.f, starting + 10.f);
        road2->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        road2->textureNames[0] = "PolygonCity_Road_01.bmp";
        road2->textureRatios[0] = 1.f;
        road2->boundingRadius = boundingRadius; 
        ::g_vec_pMeshes.push_back(road2);
    }
    {
        cMesh* road1 = new cMesh("City/SM_Env_Road_YellowLines_02.fbx");
        road1->lowDetailMeshName = "";
        road1->scale = glm::vec3(2.0f);
        road1->positionXYZ = glm::vec3(10.f, -3.f, starting - 10.f);
        road1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
        road1->textureNames[0] = "PolygonCity_Road_01.bmp";
        road1->textureRatios[0] = 1.f;
        road1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(road1);
        cMesh* road2 = new cMesh("City/SM_Env_Road_YellowLines_02.fbx");
        road2->lowDetailMeshName = "";
        road2->scale = glm::vec3(2.0f);
        road2->positionXYZ = glm::vec3(-10.f, -3.f, starting);
        road2->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        road2->textureNames[0] = "PolygonCity_Road_01.bmp";
        road2->textureRatios[0] = 1.f;
        road2->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(road2);
    }
    {
        cMesh* road1 = new cMesh("City/SM_Env_Road_YellowLines_02.fbx");
        road1->lowDetailMeshName = "";
        road1->scale = glm::vec3(2.0f);
        road1->positionXYZ = glm::vec3(10.f, -3.f, starting + 10.f);
        road1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
        road1->textureNames[0] = "PolygonCity_Road_01.bmp";
        road1->textureRatios[0] = 1.f;
        road1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(road1);
        cMesh* road2 = new cMesh("City/SM_Env_Road_YellowLines_02.fbx");
        road2->lowDetailMeshName = "";
        road2->scale = glm::vec3(2.0f);
        road2->positionXYZ = glm::vec3(-10.f, -3.f, starting + 20.f);
        road2->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        road2->textureNames[0] = "PolygonCity_Road_01.bmp";
        road2->textureRatios[0] = 1.f;
        road2->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(road2);
    }
    {
        cMesh* road1 = new cMesh("City/SM_Env_Road_YellowLines_02.fbx");
        road1->lowDetailMeshName = "";
        road1->scale = glm::vec3(2.0f);
        road1->positionXYZ = glm::vec3(10.f, -3.f, starting + 60.f);
        road1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
        road1->textureNames[0] = "PolygonCity_Road_01.bmp";
        road1->textureRatios[0] = 1.f;
        road1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(road1);
        cMesh* road2 = new cMesh("City/SM_Env_Road_YellowLines_02.fbx");
        road2->lowDetailMeshName = "";
        road2->scale = glm::vec3(2.0f);
        road2->positionXYZ = glm::vec3(-10.f, -3.f, starting + 70.f);
        road2->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        road2->textureNames[0] = "PolygonCity_Road_01.bmp";
        road2->textureRatios[0] = 1.f;
        road2->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(road2);
    }
    //Build intersecting road.
    for (float x = -50.f; x < 60.f; x += 10.f) {
        {
            cMesh* road1 = new cMesh("City/SM_Env_Road_Bare_01.fbx");
            road1->lowDetailMeshName = "";
            road1->scale = glm::vec3(2.0f);
            road1->positionXYZ = glm::vec3(x, -3.f, starting + 40.f);
            road1->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
            road1->textureNames[0] = "PolygonCity_Road_01.bmp";
            road1->textureRatios[0] = 1.f;
            road1->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(road1);
            cMesh* road2 = new cMesh("City/SM_Env_Road_Bare_01.fbx");
            road2->lowDetailMeshName = "";
            road2->scale = glm::vec3(2.0f);
            road2->positionXYZ = glm::vec3(x, -3.f, starting + 30.f);
            road2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
            road2->textureNames[0] = "PolygonCity_Road_01.bmp";
            road2->textureRatios[0] = 1.f;
            road2->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(road2);
        }
        {
            cMesh* road1 = new cMesh("City/SM_Env_Road_Bare_01.fbx");
            road1->lowDetailMeshName = "";
            road1->scale = glm::vec3(2.0f);
            road1->positionXYZ = glm::vec3(x, -3.f, starting + 50.f);
            road1->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
            road1->textureNames[0] = "PolygonCity_Road_01.bmp";
            road1->textureRatios[0] = 1.f;
            road1->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(road1);
            cMesh* road2 = new cMesh("City/SM_Env_Road_Bare_01.fbx");
            road2->lowDetailMeshName = "";
            road2->scale = glm::vec3(2.0f);
            road2->positionXYZ = glm::vec3(x, -3.f, starting + 60.f);
            road2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
            road2->textureNames[0] = "PolygonCity_Road_01.bmp";
            road2->textureRatios[0] = 1.f;
            road2->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(road2);
        }
    }
    
    {
        cMesh* side2 = new cMesh("City/SM_Env_Sidewalk_Straight_01.fbx");
        side2->lowDetailMeshName = "";
        side2->scale = glm::vec3(2.0f);
        side2->positionXYZ = glm::vec3(-10.f, -3.f, starting + 0.f);
        side2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
        side2->textureNames[0] = "PolygonCity_Texture2.bmp";
        side2->textureRatios[0] = 1.f;
        side2->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side2);
    }
    {
        cMesh* side2 = new cMesh("City/SM_Env_Sidewalk_Straight_01.fbx");
        side2->lowDetailMeshName = "";
        side2->scale = glm::vec3(2.0f);
        side2->positionXYZ = glm::vec3(-10.f, -3.f, starting + 10.f);
        side2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
        side2->textureNames[0] = "PolygonCity_Texture2.bmp";
        side2->textureRatios[0] = 1.f;
        side2->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side2);
    }
    {
        cMesh* side1 = new cMesh("City/SM_Env_Sidewalk_01.fbx");
        side1->lowDetailMeshName = "";
        side1->scale = glm::vec3(2.0f);
        side1->positionXYZ = glm::vec3(-30.f, -3.f, starting - 10.f);
        side1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
        side1->textureNames[0] = "PolygonCity_Texture2.bmp";
        side1->textureRatios[0] = 1.f;
        side1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side1);
    }
    {
        cMesh* side1 = new cMesh("City/SM_Env_Sidewalk_01.fbx");
        side1->lowDetailMeshName = "";
        side1->scale = glm::vec3(2.0f);
        side1->positionXYZ = glm::vec3(-30.f, -3.f, starting + 0.f);
        side1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
        side1->textureNames[0] = "PolygonCity_Texture2.bmp";
        side1->textureRatios[0] = 1.f;
        side1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side1);
    }
    //Corners
    {
        cMesh* corner = new cMesh("City/SM_Env_Sidewalk_Corner_01.fbx");
        corner->lowDetailMeshName = "";
        corner->scale = glm::vec3(2.0f);
        corner->positionXYZ = glm::vec3(10.f, -3.f, starting + 20.f);
        corner->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        corner->textureNames[0] = "PolygonCity_Texture2.bmp";
        corner->textureRatios[0] = 1.f;
        corner->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(corner);
        cMesh* corner2 = new cMesh("City/SM_Env_Sidewalk_Corner_01.fbx");
        corner2->lowDetailMeshName = "";
        corner2->scale = glm::vec3(2.0f);
        corner2->positionXYZ = glm::vec3(-10.f, -3.f, starting + 20.f);
        corner2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
        corner2->textureNames[0] = "PolygonCity_Texture2.bmp";
        corner2->textureRatios[0] = 1.f;
        corner2->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(corner2);
    }
    {
        cMesh* corner = new cMesh("City/SM_Env_Sidewalk_Corner_01.fbx");
        corner->lowDetailMeshName = "";
        corner->scale = glm::vec3(2.0f);
        corner->positionXYZ = glm::vec3(-10.f, -3.f, starting + 60.f);
        corner->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
        corner->textureNames[0] = "PolygonCity_Texture2.bmp";
        corner->textureRatios[0] = 1.f;
        corner->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(corner);
        cMesh* corner2 = new cMesh("City/SM_Env_Sidewalk_Corner_01.fbx");
        corner2->lowDetailMeshName = "";
        corner2->scale = glm::vec3(2.0f);
        corner2->positionXYZ = glm::vec3(10.f, -3.f, starting + 60.f);
        corner2->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
        corner2->textureNames[0] = "PolygonCity_Texture2.bmp";
        corner2->textureRatios[0] = 1.f;
        corner2->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(corner2);
    }

    for (float i = -30.f; i > -80.f; i -= 10.f) {
        cMesh* side = new cMesh("City/SM_Env_Sidewalk_Straight_01.fbx");
        side->lowDetailMeshName = "";
        side->scale = glm::vec3(2.0f);
        side->positionXYZ = glm::vec3(i, -3.f, starting + 20.f);
        side->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        side->textureNames[0] = "PolygonCity_Texture2.bmp";
        side->textureRatios[0] = 1.f;
        side->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side);
        cMesh* side1 = new cMesh("City/SM_Env_Sidewalk_01.fbx");
        side1->lowDetailMeshName = "";
        side1->scale = glm::vec3(2.0f);
        side1->positionXYZ = glm::vec3(i, -3.f, starting + 10.f);
        side1->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        side1->textureNames[0] = "PolygonCity_Texture2.bmp";
        side1->textureRatios[0] = 1.f;
        side1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side1); 
        {
            cMesh* side = new cMesh("City/SM_Env_Sidewalk_Straight_01.fbx");
            side->lowDetailMeshName = "";
            side->scale = glm::vec3(2.0f);
            side->positionXYZ = glm::vec3(i+10.f, -3.f, starting + 60.f);
            side->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
            side->textureNames[0] = "PolygonCity_Texture2.bmp";
            side->textureRatios[0] = 1.f;
            side->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(side);
            cMesh* side1 = new cMesh("City/SM_Env_Sidewalk_01.fbx");
            side1->lowDetailMeshName = "";
            side1->scale = glm::vec3(2.0f);
            side1->positionXYZ = glm::vec3(i+10.f, -3.f, starting + 70.f);
            side1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
            side1->textureNames[0] = "PolygonCity_Texture2.bmp";
            side1->textureRatios[0] = 1.f;
            side1->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(side1);
        }
    }
    for (float i = 20.f; i < 80.f; i += 10.f) {
        cMesh* side = new cMesh("City/SM_Env_Sidewalk_Straight_01.fbx");
        side->lowDetailMeshName = "";
        side->scale = glm::vec3(2.0f);
        side->positionXYZ = glm::vec3(i, -3.f, starting + 20.f);
        side->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        side->textureNames[0] = "PolygonCity_Texture2.bmp";
        side->textureRatios[0] = 1.f;
        side->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side);
        cMesh* side1 = new cMesh("City/SM_Env_Sidewalk_01.fbx");
        side1->lowDetailMeshName = "";
        side1->scale = glm::vec3(2.0f);
        side1->positionXYZ = glm::vec3(i, -3.f, starting + 10.f);
        side1->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        side1->textureNames[0] = "PolygonCity_Texture2.bmp";
        side1->textureRatios[0] = 1.f;
        side1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side1);
        {
            cMesh* side = new cMesh("City/SM_Env_Sidewalk_Straight_01.fbx");
            side->lowDetailMeshName = "";
            side->scale = glm::vec3(2.0f);
            side->positionXYZ = glm::vec3(i + 10.f, -3.f, starting + 60.f);
            side->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
            side->textureNames[0] = "PolygonCity_Texture2.bmp";
            side->textureRatios[0] = 1.f;
            side->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(side);
            cMesh* side1 = new cMesh("City/SM_Env_Sidewalk_01.fbx");
            side1->lowDetailMeshName = "";
            side1->scale = glm::vec3(2.0f);
            side1->positionXYZ = glm::vec3(i + 10.f, -3.f, starting + 70.f);
            side1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
            side1->textureNames[0] = "PolygonCity_Texture2.bmp";
            side1->textureRatios[0] = 1.f;
            side1->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(side1);
        }
    }
    std::string door, stack, roof;
    for (int x = 65.f; x < 105.f; x+=20.f)
    {
    GetBuildingParts(door, stack, roof);
    cMesh* door1 = new cMesh(door);
    door1->lowDetailMeshName = "";
    door1->scale = glm::vec3(4.0f);
    door1->positionXYZ = glm::vec3(x, -3.f, (float)starting+70.f);
    door1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
    door1->textureNames[0] = "PolygonCity_Texture2.bmp";
    door1->textureRatios[0] = 1.f;
    door1->boundingRadius = boundingRadius;
    ::g_vec_pMeshes.push_back(door1);
    cMesh* stack1 = new cMesh(stack);
    stack1->lowDetailMeshName = "";
    stack1->scale = glm::vec3(4.0f);
    stack1->positionXYZ = glm::vec3(x, 7.f, (float)starting + 70.f);
    stack1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
    stack1->textureNames[0] = "PolygonCity_Texture2.bmp";
    stack1->textureRatios[0] = 1.f;
    stack1->boundingRadius = boundingRadius;
    ::g_vec_pMeshes.push_back(stack1);
    cMesh* roof1 = new cMesh(roof);
    roof1->lowDetailMeshName = "";
    roof1->scale = glm::vec3(4.0f);
    roof1->positionXYZ = glm::vec3(x, 42.f, (float)starting + 70.f);
    roof1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
    roof1->textureNames[0] = "PolygonCity_Texture2.bmp";
    roof1->textureRatios[0] = 1.f;
    roof1->boundingRadius = boundingRadius;
    ::g_vec_pMeshes.push_back(roof1); 
    }
    for (int x = -105.f; x <= -45.f; x += 20.f)
    {
        GetBuildingParts(door, stack, roof);
        cMesh* door1 = new cMesh(door);
        door1->lowDetailMeshName = "";
        door1->scale = glm::vec3(4.0f);
        door1->positionXYZ = glm::vec3(x, -3.f, (float)starting + 70.f);
        door1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
        door1->textureNames[0] = "PolygonCity_Texture2.bmp";
        door1->textureRatios[0] = 1.f;
        door1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(door1);
        cMesh* stack1 = new cMesh(stack);
        stack1->lowDetailMeshName = "";
        stack1->scale = glm::vec3(4.0f);
        stack1->positionXYZ = glm::vec3(x, 7.f, (float)starting + 70.f);
        stack1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
        stack1->textureNames[0] = "PolygonCity_Texture2.bmp";
        stack1->textureRatios[0] = 1.f;
        stack1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(stack1);
        cMesh* roof1 = new cMesh(roof);
        roof1->lowDetailMeshName = "";
        roof1->scale = glm::vec3(4.0f);
        roof1->positionXYZ = glm::vec3(x, 42.f, (float)starting + 70.f);
        roof1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
        roof1->textureNames[0] = "PolygonCity_Texture2.bmp";
        roof1->textureRatios[0] = 1.f;
        roof1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(roof1);
    }

    //Now that we are done with the intersection, build ahead.
    BuildStreet((int)(starting + 70), (int)(starting + 670));
}


void DecorationManager::BuildStreet(int startZ, int endZ) {
    float boundingRadius = -1.f;
    bool drawBuildings = true;
    std::string door, stack, roof;
    for (int currentZ = startZ; currentZ < endZ; currentZ += 10)
    {
        if (drawBuildings && currentZ < startZ + 430) {
            GetBuildingParts(door, stack, roof);
            cMesh* door1 = new cMesh(door);
            door1->lowDetailMeshName = "";
            door1->scale = glm::vec3(4.0f);
            door1->positionXYZ = glm::vec3(25.f, -3.f, (float)currentZ);
            door1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
            door1->textureNames[0] = "PolygonCity_Texture2.bmp";
            door1->textureRatios[0] = 1.f;
            door1->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(door1);
            cMesh* stack1 = new cMesh(stack);
            stack1->lowDetailMeshName = "";
            stack1->scale = glm::vec3(4.0f);
            stack1->positionXYZ = glm::vec3(25.f, 7.f, (float)currentZ);
            stack1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
            stack1->textureNames[0] = "PolygonCity_Texture2.bmp";
            stack1->textureRatios[0] = 1.f;
            stack1->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(stack1);
            cMesh* roof1 = new cMesh(roof);
            roof1->lowDetailMeshName = "";
            roof1->scale = glm::vec3(4.0f);
            roof1->positionXYZ = glm::vec3(25.f, 42.f, (float)currentZ);
            roof1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
            roof1->textureNames[0] = "PolygonCity_Texture2.bmp";
            roof1->textureRatios[0] = 1.f;
            roof1->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(roof1);

            GetBuildingParts(door, stack, roof);
            cMesh* door2 = new cMesh(door);
            door2->lowDetailMeshName = "";
            door2->scale = glm::vec3(4.0f);
            door2->positionXYZ = glm::vec3(-25.f, -3.f, (float)currentZ + 20.f);
            door2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
            door2->textureNames[0] = "PolygonCity_Texture2.bmp";
            door2->textureRatios[0] = 1.f;
            door2->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(door2);
            cMesh* stack2 = new cMesh(stack);
            stack2->lowDetailMeshName = "";
            stack2->scale = glm::vec3(4.0f);
            stack2->positionXYZ = glm::vec3(-25.f, 7.f, (float)currentZ + 20.f);
            stack2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
            stack2->textureNames[0] = "PolygonCity_Texture2.bmp";
            stack2->textureRatios[0] = 1.f;
            stack2->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(stack2);
            cMesh* roof2 = new cMesh(roof);
            roof2->lowDetailMeshName = "";
            roof2->scale = glm::vec3(4.0f);
            roof2->positionXYZ = glm::vec3(-25.f, 42.f, (float)currentZ + 20.f);
            roof2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
            roof2->textureNames[0] = "PolygonCity_Texture2.bmp";
            roof2->textureRatios[0] = 1.f;
            roof2->boundingRadius = boundingRadius;
            ::g_vec_pMeshes.push_back(roof2);
            drawBuildings = false;
        }
        else {
            drawBuildings = true;
        }
        cMesh* side1extra = new cMesh("City/SM_Env_Sidewalk_Straight_01.fbx");
        side1extra->lowDetailMeshName = "";
        side1extra->scale = glm::vec3(2.0f);
        side1extra->positionXYZ = glm::vec3(30.f, -3.f, (float)currentZ + 10.f);
        side1extra->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
        side1extra->textureNames[0] = "PolygonCity_Texture2.bmp";
        side1extra->textureRatios[0] = 1.f;
        side1extra->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side1extra);
        cMesh* side1 = new cMesh(ShouldAddVent(currentZ, true) ? "City/SM_Env_Sidewalk_Gutter_01.fbx" : "City/SM_Env_Sidewalk_Straight_01.fbx");
        side1->lowDetailMeshName = "";
        side1->scale = glm::vec3(2.0f);
        side1->positionXYZ = glm::vec3(10.f, -3.f, (float)currentZ);
        side1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
        side1->textureNames[0] = "PolygonCity_Texture2.bmp";
        side1->textureRatios[0] = 1.f;
        side1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side1);
        cMesh* road1 = new cMesh("City/SM_Env_Road_YellowLines_02.fbx");
        road1->lowDetailMeshName = "";
        road1->scale = glm::vec3(2.0f);
        road1->positionXYZ = glm::vec3(10.f, -3.f, (float)currentZ);
        road1->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
        road1->textureNames[0] = "PolygonCity_Road_01.bmp";
        road1->textureRatios[0] = 1.f;
        road1->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(road1);
        cMesh* road2 = new cMesh("City/SM_Env_Road_YellowLines_02.fbx");
        road2->lowDetailMeshName = "";
        road2->scale = glm::vec3(2.0f);
        road2->positionXYZ = glm::vec3(-10.f, -3.f, (float)currentZ + 10.f);
        road2->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
        road2->textureNames[0] = "PolygonCity_Road_01.bmp";
        road2->textureRatios[0] = 1.f;
        road2->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(road2);
        cMesh* side2 = new cMesh(ShouldAddVent(currentZ, false) ? "City/SM_Env_Sidewalk_Gutter_01.fbx" : "City/SM_Env_Sidewalk_Straight_01.fbx");
        side2->lowDetailMeshName = "";
        side2->scale = glm::vec3(2.0f);
        side2->positionXYZ = glm::vec3(-10.f, -3.f, (float)currentZ + 10.f);
        side2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2.f, 0.f);
        side2->textureNames[0] = "PolygonCity_Texture2.bmp";
        side2->textureRatios[0] = 1.f;
        side2->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side2);
        cMesh* side2extra = new cMesh("City/SM_Env_Sidewalk_Straight_01.fbx");
        side2extra->lowDetailMeshName = "";
        side2extra->scale = glm::vec3(2.0f);
        side2extra->positionXYZ = glm::vec3(-30.f, -3.f, (float)currentZ);
        side2extra->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2.f, 0.f);
        side2extra->textureNames[0] = "PolygonCity_Texture2.bmp";
        side2extra->textureRatios[0] = 1.f;
        side2extra->boundingRadius = boundingRadius;
        ::g_vec_pMeshes.push_back(side2extra);
    }

}

void DecorationManager::AddVents() {
    for (int i : leftVents) {
        cParticleEmitter* vent = new cParticleEmitter(PARTICLE_BUBBLE, glm::vec3(10.f, -0.1f, (float)i), 0.5f, glm::vec3(0.f, 3.0f, 0.f), 0.2f, 0.1f, 0.7f);
        vents.push_back(vent);
    }
    for (int i : rightVents) {
        cParticleEmitter* vent = new cParticleEmitter(PARTICLE_BUBBLE, glm::vec3(-10.f, -0.1f, (float)i), 0.5f, glm::vec3(0.f, 3.0f, 0.f), 0.2f, 0.1f, 0.7f);
        vents.push_back(vent);
    }
}

void DecorationManager::TimeStep(float deltaTime) {
    for (cParticleEmitter* vent : vents) {
        vent->TimeStep(deltaTime);
    }
}

bool DecorationManager::ShouldAddVent(int location, bool onLeft) {
    //Number of vents is small so can stick with vectors. May want other data structure if this is going to grow.
    if (onLeft) {
        for (int i : leftVents) {
            if (i == location) {
                return true;
            }
        }
        return false;
    }
    else {
        for (int i : rightVents) {
            if (i == location) {
                return true;
            }
        }
        return false;
    }
}