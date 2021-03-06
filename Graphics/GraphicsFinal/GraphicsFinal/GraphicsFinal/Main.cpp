#pragma once

#include <iostream>
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp> // glm::vec3
#include <extern/glm/vec4.hpp> // glm::vec4
#include <extern/glm/mat4x4.hpp> // glm::mat4
#include <extern/glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <extern/glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "globals.h"
#include <algorithm>
#include <Physics/cWorldSpace.h>

cWorldSpace* worldSpace = cWorldSpace::Instance();

// Function signature for DrawObject()
void DrawObject(
    cMesh* pCurrentMesh,
    glm::mat4 matModel,
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLuint program,
    cVAOManager* pVAOManager);


void DrawDebugObjects(
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLuint program,
    cVAOManager* pVAOManager);

void InitShaders(GLuint& program, cShaderManager::cShader& vertShader, cShaderManager::cShader& geomShader, cShaderManager::cShader& fragShader) {
    // Hardcoding shader file names. May try to place in a common location in the future / make configurable.
    vertShader.fileName = "assets/shaders/vertShader_01.glsl";
    geomShader.fileName = "assets/shaders/geomShader_01.glsl";
    fragShader.fileName = "assets/shaders/fragShader_01.glsl";

    if (::g_pShaderManager->createProgramFromFile("Shader#1", vertShader, geomShader, fragShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        program = ::g_pShaderManager->getIDFromFriendlyName("Shader#1");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
    }
}

void Shutdown(GLFWwindow* pWindow){
    ::g_ShutDown(pWindow);
    glfwDestroyWindow(pWindow);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

int main(void) {
    //Consider separating regions into separate methods as 'main' grows.
    #pragma region Intialization
    // Initialize the window.
    GLFWwindow* pWindow;
    GLuint program = 0;     // 0 means "no shader program"
    GLint mvp_location = -1;

    if (!glfwInit())
    {
        return -1;
        //exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    ::g_TitleText = "Curtis Tremblay - INFO-6028 Final";
    pWindow = glfwCreateWindow(1200, 640, ::g_TitleText.c_str(), NULL, NULL);

    if (!pWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(pWindow, GLFW_key_callback);
    glfwSetCursorEnterCallback(pWindow, GLFW_cursor_enter_callback);
    glfwSetCursorPosCallback(pWindow, GLFW_cursor_position_callback);
    glfwSetScrollCallback(pWindow, GLFW_scroll_callback);
    glfwSetMouseButtonCallback(pWindow, GLFW_mouse_button_callback);
    glfwSetWindowSizeCallback(pWindow, GLFW_window_size_callback);

    glfwMakeContextCurrent(pWindow);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    
    // See how many active uniform variables (registers) I can have
    GLint max_uniform_location = 0;
    GLint* p_max_uniform_location = NULL;
    p_max_uniform_location = &max_uniform_location;
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);

    std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max_uniform_location << std::endl;

    ::g_StartUp(pWindow);

    ::g_pFlyCamera->setEye(::g_pConfigManager->_cameraStartingPosition);
    // Eventually figure out how to adjust orientation of camera so it doesn't 'jump' on first movement.
    //::g_pFlyCamera->setAt(::g_pConfigManager->_cameraStartingOrientation);

    cShaderManager::cShader vertShader;
    cShaderManager::cShader fragShader;
    cShaderManager::cShader geomShader;
    InitShaders(program, vertShader, geomShader, fragShader);

    glUseProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    GLint matModel_Location = glGetUniformLocation(program, "matModel");
    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
    GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");
    #pragma endregion

    #pragma region Lights
    ::g_pTheLights->theLights[0].position = glm::vec4(0.f, 5000.f, 0.f, 1.f);
    ::g_pTheLights->theLights[0].direction = glm::vec4(0.f, -1.f, 0.f, 1.f);
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(1.f, 1.f, 0.95f, 1.f);
    ::g_pTheLights->theLights[0].param1.x = 2.f;    // Directional light
    ::g_pTheLights->TurnOnLight(0);

    // Get the uniform locations of the light shader values
    ::g_pTheLights->SetUpUniformLocations(program);

    #pragma endregion

    #pragma region Debug


    #if defined _DEBUG
    // Set up the debug sphere object
    ::g_pDebugSphere = new cMesh();
    ::g_pDebugSphere->meshName = "Sphere_xyz_n_rgba_uv.ply";
    ::g_pDebugSphere->bIsWireframe = true;
    ::g_pDebugSphere->bUseObjectDebugColour = true;
    ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pDebugSphere->bDontLight = true;
    ::g_pDebugSphere->scale = 1.0f;
    ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
    // Give this a friendly name
    ::g_pDebugSphere->friendlyName = "Debug Sphere"; 

    #endif

    #pragma endregion

    #pragma region Models
    ::g_pVAOManager->setFilePath("assets/models/");
    std::vector<std::string> vecModelsToLoad;
    vecModelsToLoad.push_back("Sphere_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("ISO_Shphere_flat_3div_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
    vecModelsToLoad.push_back("tile.ply");
    vecModelsToLoad.push_back("Floors/SM_Env_Dwarf_Floor_03.ply");
    vecModelsToLoad.push_back("Walls/SM_Env_Dwarf_Wall_01.ply");
    vecModelsToLoad.push_back("Doors/SM_Env_Dwarf_Wall_DoorFrame_Single_01.ply");
    vecModelsToLoad.push_back("Doors/SM_Env_Dwarf_Wall_DoorFrame_Single_01_Door.ply");
    vecModelsToLoad.push_back("Doors/SM_Env_Dwarf_Wall_DoorFrame_Slider_01.ply");
    vecModelsToLoad.push_back("Doors/SM_Env_Dwarf_Wall_DoorFrame_Slider_01_Door.ply");

    vecModelsToLoad.push_back("Crystals/SM_Env_Crystals_Cluster_Large_01.ply");
    vecModelsToLoad.push_back("Crystals/SM_Env_Crystals_Cluster_Large_02.ply");
    vecModelsToLoad.push_back("Crystals/SM_Env_Crystals_Cluster_Large_03.ply");
    vecModelsToLoad.push_back("Crystals/SM_Env_Crystals_Cluster_Large_04.ply");

    vecModelsToLoad.push_back("Plants/SM_Env_Plant_01.ply");
    vecModelsToLoad.push_back("Plants/SM_Env_Plant_02.ply");
    vecModelsToLoad.push_back("Plants/SM_Env_Plant_Spikey_01.ply");
    vecModelsToLoad.push_back("Plants/SM_Env_Plants_01.ply");
    vecModelsToLoad.push_back("Plants/SM_Env_Plants_02.ply");
    vecModelsToLoad.push_back("Plants/SM_Env_Plants_03.ply");
    vecModelsToLoad.push_back("Plants/SM_Env_Tree_Dead_01.ply");
    vecModelsToLoad.push_back("Plants/SM_Generic_Tree_02.ply");
    vecModelsToLoad.push_back("Plants/SM_Generic_Tree_04.ply"); 

    vecModelsToLoad.push_back("SM_Prop_Dwarf_Torch_05.ply");
    vecModelsToLoad.push_back("SM_Env_Dwarf_Stairs_01.ply");

    vecModelsToLoad.push_back("FX_SunShafts.ply");

    unsigned int totalVerticesLoaded = 0;
    unsigned int totalTrianglesLoaded = 0;
    for (std::vector<std::string>::iterator itModel = vecModelsToLoad.begin(); itModel != vecModelsToLoad.end(); itModel++)
    {
        sModelDrawInfo theModel;
        std::string modelName = *itModel;
        std::vector<sVertex> vecVertexArray;
        std::cout << "Loading " << modelName << "...";
        if (!::g_pVAOManager->LoadModelIntoVAO(modelName, theModel, program, vecVertexArray))
        {
            std::cout << "didn't work because: " << std::endl;
            std::cout << ::g_pVAOManager->getLastError(true) << std::endl;
        }
        else
        {
            std::cout << "OK." << std::endl;
            std::cout << "\t" << theModel.numberOfVertices << " vertices and " << theModel.numberOfTriangles << " triangles loaded." << std::endl;
            totalTrianglesLoaded += theModel.numberOfTriangles;
            totalVerticesLoaded += theModel.numberOfVertices;
        }
    }

    std::cout << "Done loading models." << std::endl;
    std::cout << "Total vertices loaded = " << totalVerticesLoaded << std::endl;
    std::cout << "Total triangles loaded = " << totalTrianglesLoaded << std::endl;
    #pragma endregion

    #pragma region Textures
    // Load the textures
    ::g_pTextureManager->SetBasePath("assets/textures");
    
    ::g_pTextureManager->Create2DTextureFromBMPFile("Dungeons_2_Texture_01_A.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("spotlight.bmp", true);

    // Add a skybox texture
    std::string errorTextString;
    ::g_pTextureManager->SetBasePath("assets/textures/cubemaps");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Gold",
        "Gold_Diffuse.bmp",    /* posX_fileName */
        "Gold_Diffuse.bmp",     /*negX_fileName */
        "Gold_Diffuse.bmp",     /*negY_fileName*/
        "Gold_Diffuse.bmp",       /*posY_fileName*/
        "Gold_Diffuse.bmp",    /*posZ_fileName*/
        "Gold_Diffuse.bmp",      /*negZ_fileName*/
        true, errorTextString))
    {
        std::cout << "Didn't load because: " << errorTextString << std::endl;
    }
    else
    {
        std::cout << "Loaded the skybox cube texture OK" << std::endl;
    }
    #pragma endregion

#pragma region Dungeon
    ::g_pDungeonMaster->BuildDungeonFromTSV("Question1/The Chambers of Demonic Souls 01.txt");
    ::g_vec_pMeshes = ::g_pDungeonMaster->dungeonModels;

#pragma endregion

#pragma region Objects

    // TODO: If this has a large performance impact as scene grows, refactor.
    // Move objects with transparency to their own vector.
    for (cMesh* mesh : ::g_vec_pMeshes) {
        if (mesh->alphaTransparency < 1.f) {
            ::g_vec_pMeshesTransparency.push_back(mesh);
        }
    }
    // Now that we have all of our transparent objects, remove from the original vector.
    std::vector<cMesh*>::iterator location;
    for (cMesh* mesh : ::g_vec_pMeshesTransparency) {
        location = std::find(g_vec_pMeshes.begin(), g_vec_pMeshes.end(), mesh);
        if (location != g_vec_pMeshes.end()) {
            ::g_vec_pMeshes.erase(location);
        }
    }
    cMesh* pSkybox = new cMesh();

    // Mimics a skybox
    pSkybox->meshName = "Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply";
    pSkybox->scale = 5'000'000.0f;

    pSkybox->positionXYZ = ::g_pFlyCamera->getEye();

#pragma endregion
    const double MAX_DELTA_TIME = 0.05;  // 50 ms
    double previousTime = glfwGetTime();
    std::vector<cMesh*> sortedTransparentObjects;

    while (!glfwWindowShouldClose(pWindow)) {
        float ratio;
        int width, height;
        glm::mat4 matModel;             // used to be "m"; Sometimes it's called "world"
        glm::mat4 matProjection;        // used to be "p";
        glm::mat4 matView;              // used to be "v";

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
        previousTime = currentTime;

        glfwGetFramebufferSize(pWindow, &width, &height);
        ratio = width / (float)height;

        // Turn on the depth buffer
        glEnable(GL_DEPTH);         // Turns on the depth buffer
        glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // *******************************************************
        // Screen is cleared and we are ready to draw the scene...
        // *******************************************************

        // Update the title text
        glfwSetWindowTitle(pWindow, ::g_TitleText.c_str());
        ::g_pTheLights->FlickerCandles();
        ::g_pTheLights->DayTimeStep(deltaTime);

        // Copy the light information into the shader to draw the scene
        ::g_pTheLights->CopyLightInfoToShader();

        #if defined _DEBUG
        ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
        #endif

        matProjection = glm::perspective(
            ::g_pFlyCamera->FOV,
            ratio,
            ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
            ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

        //::g_pFlyCamera->Update(deltaTime);

        for (iEntity* actor : ::g_vec_pActors) {
            actor->Update(deltaTime);
        }
        worldSpace->_world->Update(deltaTime);

        glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
        glm::vec3 cameraAt = ::g_pFlyCamera->getAtInWorldSpace();
        glm::vec3 cameraUp = ::g_pFlyCamera->getUpVector();

        matView = glm::mat4(1.0f);
        matView = glm::lookAt(cameraEye,   // "eye"
            cameraAt,    // "at"
            cameraUp);

        glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(matView));
        glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));

        // **********************************************************************
        // Draw the "scene" of all objects.
        // i.e. go through the vector and draw each one...
        // **********************************************************************
        // Disable GL_Blend while we draw non-transparent objects
        glDisable(GL_BLEND);

        for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
        {
            // So the code is a little easier...
            cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

            DrawObject(pCurrentMesh,
                matModel,
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                ::g_pVAOManager);


        }//for (unsigned int index
        // Scene is drawn
        // **********************************************************************   

        #if defined _DEBUG

        DrawDebugObjects(matModel_Location, matModelInverseTranspose_Location, program, ::g_pVAOManager);

        #endif

        // After drawing the other objects, draw the skybox to limit overdraw.

        GLint bIsSkyBox_LocID = glGetUniformLocation(program, "bIsSkyBox");
        glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);

        // Move the "skybox object" with the camera
        pSkybox->positionXYZ = ::g_pFlyCamera->getEye();
        DrawObject(
            pSkybox, glm::mat4(1.0f),
            matModel_Location, matModelInverseTranspose_Location,
            program, ::g_pVAOManager);

        glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);

        // Now that non-transparent objects are drawn, sort and draw transparent objects.
        glEnable(GL_BLEND);

        // Utilize lambda function to assist with sorting (only if there is more than one transparent object.
        if (::g_vec_pMeshesTransparency.size() >= 2) {
            std::sort(::g_vec_pMeshesTransparency.begin(), ::g_vec_pMeshesTransparency.end(), [](const cMesh* a, const cMesh* b) -> bool
            {
                return glm::distance(a->positionXYZ, ::g_pFlyCamera->getEye()) > glm::distance(b->positionXYZ, ::g_pFlyCamera->getEye());
            });
        }

        for (unsigned int index = 0; index != ::g_vec_pMeshesTransparency.size(); index++)
        {
            // So the code is a little easier...
            cMesh* pCurrentMesh = ::g_vec_pMeshesTransparency[index];

            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

            DrawObject(pCurrentMesh,
                matModel,
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                ::g_pVAOManager);
        }

        // "Present" what we've drawn.
        glfwSwapBuffers(pWindow);        // Show what we've drawn

        // Process any events that have happened
        glfwPollEvents();

        // Handle OUR keyboard, mouse stuff
        handleAsyncKeyboard(pWindow, deltaTime);
        handleAsyncMouse(pWindow, deltaTime);
    }

    Shutdown(pWindow);
}

void DrawDebugObjects(
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLuint program,
    cVAOManager* pVAOManager)
{    

    {   // Draw a sphere where the camera is looking.
        // Take the at - eye --> vector 
        glm::vec3 eye = ::g_pFlyCamera->getEye();
        glm::vec3 at = ::g_pFlyCamera->getAtInWorldSpace();

        glm::vec3 deltaDirection = at - eye;
        // Normalize to make this vector 1.0 units in length
        deltaDirection = glm::normalize(deltaDirection);

        float SphereDistanceFromCamera = 300.0f;
        glm::vec3 sphereLocation =
            eye + (deltaDirection * SphereDistanceFromCamera);

        // Draw the sphere

        ::g_pDebugSphere->positionXYZ = sphereLocation;
        ::g_pDebugSphere->scale = 5.0f;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

        DrawObject(::g_pDebugSphere,
            glm::mat4(1.0f),
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);

    }//Draw a sphere where the camera is looking.



    if (::g_bShowDebugShere)
    {
        // Draw other things, like debug objects, UI, whatever
        glm::mat4 matModelDS = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

        // Draw a small white shere at the location of the light
        sLight& currentLight = ::g_pTheLights->theLights[::g_selectedLight];

        ::g_pDebugSphere->positionXYZ = currentLight.position;

        ::g_pDebugSphere->scale = 0.1f;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        DrawObject(::g_pDebugSphere,
            matModelDS,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);

        // Save old debug sphere model name
        std::string oldDebugSphereModel = ::g_pDebugSphere->meshName;

        const float LOW_RES_SPHERE_DISTANCE = 50.0f;
        const std::string LOW_RES_SPHERE_MODEL = "ISO_Sphere_flat_3div_xyz_n_rgba_uv.ply";
        const std::string HIGH_RES_SPHERE_MODEL = "ISO_Sphere_flat_4div_xyz_n_rgba_uv.ply";

        float distTo95Percent = ::g_pTheLights->lightHelper.calcApproxDistFromAtten(0.95f,    /* the target light level I want*/
            0.01f,    /*accuracy - how close to 0.25f*/
            10000.0f, /*infinity away*/
            currentLight.atten.x, /*const atten*/
            currentLight.atten.y, /*linear atten*/
            currentLight.atten.z, /*quadratic atten*/
            cLightHelper::DEFAULTMAXITERATIONS);
        // Draw a red sphere at 95%
        ::g_pDebugSphere->scale = distTo95Percent;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

        ::g_pDebugSphere->meshName = (::g_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);

        DrawObject(::g_pDebugSphere,
            matModelDS,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);

        // How far away is 50% brightness?
        float distTo50Percent = ::g_pTheLights->lightHelper.calcApproxDistFromAtten(0.50f,    /* the target light level I want*/
            0.01f,    /*accuracy - how close to 0.25f*/
            10000.0f, /*infinity away*/
            currentLight.atten.x, /*const atten*/
            currentLight.atten.y, /*linear atten*/
            currentLight.atten.z, /*quadratic atten*/
            cLightHelper::DEFAULTMAXITERATIONS);
        // Draw a red sphere at 50%
        ::g_pDebugSphere->scale = distTo50Percent;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        ::g_pDebugSphere->meshName = (::g_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(::g_pDebugSphere,
            matModelDS,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);

        // How far away is 25% brightness?
        float distTo25Percent = ::g_pTheLights->lightHelper.calcApproxDistFromAtten(0.25f,    /* the target light level I want*/
            0.01f,    /*accuracy - how close to 0.25f*/
            10000.0f, /*infinity away*/
            currentLight.atten.x, /*const atten*/
            currentLight.atten.y, /*linear atten*/
            currentLight.atten.z, /*quadratic atten*/
            cLightHelper::DEFAULTMAXITERATIONS);
        // Draw a red sphere at 25%
        ::g_pDebugSphere->scale = distTo25Percent;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        ::g_pDebugSphere->meshName = (::g_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(::g_pDebugSphere,
            matModelDS,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);

        // How far away is 5% brightness?
        float distTo5Percent = ::g_pTheLights->lightHelper.calcApproxDistFromAtten(0.05f,    /* the target light level I want*/
            0.01f,    /*accuracy - how close to 0.25f*/
            10000.0f, /*infinity away*/
            currentLight.atten.x, /*const atten*/
            currentLight.atten.y, /*linear atten*/
            currentLight.atten.z, /*quadratic atten*/
            cLightHelper::DEFAULTMAXITERATIONS);
        // Draw a red sphere at 5%
        ::g_pDebugSphere->scale = distTo5Percent;
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        ::g_pDebugSphere->meshName = (::g_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(::g_pDebugSphere,
            matModelDS,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);

        ::g_pDebugSphere->meshName = oldDebugSphereModel;

    }//if ( ::g_bShowDebugShere )

    return;
}