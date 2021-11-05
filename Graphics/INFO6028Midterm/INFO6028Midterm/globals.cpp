#include "globals.h"

cFlyCamera* g_pFlyCamera = NULL;

//glm::vec3 g_cameraEye = glm::vec3(0.0f, 0.0f, +4.0f);
//glm::vec3 g_cameraAt = glm::vec3(0.0f, 0.0f, 0.0f);

cVAOManager* g_pVAOManager = NULL;
cShaderManager* g_pShaderManager = NULL;

cLightManager* g_pTheLights = NULL;

std::vector< cMesh* > g_vec_pMeshes;

cMesh* g_pDebugSphere = NULL;
bool g_bShowDebugShere = false;


unsigned int g_selectedObject = 0;
unsigned int g_selectedLight = 0;

bool g_doorsOpened = false;
bool g_doorsMoving = false;

std::vector<unsigned int> wallPanel1;
std::vector<unsigned int> wallPanel2;
std::vector<unsigned int> wallPanel3;
std::vector<unsigned int> wallPanel4;


// This will be printed in the title bar
std::string g_TitleText = "";


bool g_FindObjectByUniqueID(unsigned int uniqueID_toFind, cMesh*& theObjectIFound);



cMesh* g_findObjectByUniqueID(unsigned int uniqueID_toFind)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        if (::g_vec_pMeshes[index]->getUniqueID() == uniqueID_toFind)
        {
            return ::g_vec_pMeshes[index];
        }
    }
    return NULL;
}

bool g_findObjectByUniqueID(unsigned int uniqueID_toFind, cMesh& theObjectIFound)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        if (::g_vec_pMeshes[index]->getUniqueID() == uniqueID_toFind)
        {
            // Dereferences and returns a stack based copy
            theObjectIFound = *(::g_vec_pMeshes[index]);
            return true;
        }
    }
    return false;
}

bool g_FindObjectByUniqueID(unsigned int uniqueID_toFind, cMesh*& theObjectIFound)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        if (::g_vec_pMeshes[index]->getUniqueID() == uniqueID_toFind)
        {
            // Returns a pointer to the object
            theObjectIFound = ::g_vec_pMeshes[index];
            return true;
        }
    }
    return false;
}

// If not found, return NULL (0)
cMesh* g_findObjectByFriendlyName(std::string NametoFind)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        cMesh* pMeshToTest = ::g_vec_pMeshes[index];

        // Is functionally the same thing as using a reference:
        //cMesh& meshToTest = g_vecMeshes[index];

        if (pMeshToTest->friendlyName == NametoFind)
        {
            return pMeshToTest;
        }
    }
    // DIDN'T find it, return NULL
    return NULL;
}

cMesh* g_FindObjectByUniqueID(unsigned int uniqueID_toFind)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        cMesh* pMeshToTest = ::g_vec_pMeshes[index];

        // Is functionally the same thing as using a reference:
        //cMesh& meshToTest = g_vecMeshes[index];

        if (pMeshToTest->getUniqueID() == uniqueID_toFind)
        {
            return pMeshToTest;
        }
    }
    // DIDN'T find it, return NULL
    return NULL;
}

void g_OpenDoor(double deltaTime) {
    bool stillMoving = false;
    if (g_FindObjectByUniqueID(wallPanel2[0])->positionXYZ.x > g_FindObjectByUniqueID(wallPanel1[0])->positionXYZ.x) {
        stillMoving = true;
        for (unsigned int index : wallPanel2) {
            g_FindObjectByUniqueID(index)->positionXYZ.x += deltaTime * 5;
        }
    }
    if (g_FindObjectByUniqueID(wallPanel3[0])->positionXYZ.x > g_FindObjectByUniqueID(wallPanel1[0])->positionXYZ.x) {
        stillMoving = true;
        for (unsigned int index : wallPanel3) {
            g_FindObjectByUniqueID(index)->positionXYZ.x += deltaTime * 10;
        }
    }
    if (g_FindObjectByUniqueID(wallPanel4[0])->positionXYZ.x > g_FindObjectByUniqueID(wallPanel1[0])->positionXYZ.x) {
        stillMoving = true;
        for (unsigned int index : wallPanel4) {
            g_FindObjectByUniqueID(index)->positionXYZ.x += deltaTime * 15;
        }
    }
    if (stillMoving) {
        glm::vec3 vectorZ = glm::vec3(0.f, 0.f, 10.f * (sin(2 * glm::pi<double>() * deltaTime)));
        glm::vec3 vectorX = glm::vec3((10.f * cos(2 * glm::pi<double>() * deltaTime)), 0.f, 0.f);

        ::g_pTheLights->MoveLightRelative(11, vectorZ + vectorX);
    }
    else {
        g_pTheLights->TurnOffLight(11);
        g_doorsMoving = false;
        g_doorsOpened = true;
    }
}

void g_CloseDoor(double deltaTime) {
    bool stillMoving = false;
    if (g_FindObjectByUniqueID(wallPanel2[0])->positionXYZ.x < 25.f) {
        stillMoving = true;
        for (unsigned int index : wallPanel2) {
            g_FindObjectByUniqueID(index)->positionXYZ.x -= deltaTime * 5;
        }
    }
    if (g_FindObjectByUniqueID(wallPanel3[0])->positionXYZ.x < 125.f) {
        stillMoving = true;
        for (unsigned int index : wallPanel3) {
            g_FindObjectByUniqueID(index)->positionXYZ.x -= deltaTime * 10;
        }
    }
    if (g_FindObjectByUniqueID(wallPanel4[0])->positionXYZ.x < 225.f) {
        stillMoving = true;
        for (unsigned int index : wallPanel4) {
            g_FindObjectByUniqueID(index)->positionXYZ.x -= deltaTime * 15;
        }
    }
    if (!stillMoving) {
        g_pTheLights->TurnOffLight(11);
        g_doorsMoving = false;
        g_doorsOpened = false;
    }
}
