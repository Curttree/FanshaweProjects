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

void InitShaders(GLuint& program, cShaderManager::cShader& vertShader, cShaderManager::cShader& fragShader) {
    // Hardcoding shader file names. May try to place in a common location in the future / make configurable.
    vertShader.fileName = "assets/shaders/vertShader_01.glsl";
    fragShader.fileName = "assets/shaders/fragShader_01.glsl";

    if (::g_pShaderManager->createProgramFromFile("Shader#1", vertShader, fragShader))
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

void initArrow() {
    arrowModel = new cMesh();
    arrowModel->meshName = "Quad_x3_2_sided_axial_imposter_base_on_XY_axis.ply";
    arrowModel->scale = 10.f;
    arrowModel->positionXYZ = ::g_pConfigManager->_shotPositions[0];
    arrowModel->textureNames[8] = "arrow.bmp";
    arrowModel->textureRatios[8] = 1.f;
    arrowModel->bUseDiscardTransparency = true;
    arrowModel->discardColour = glm::vec3(0.f);
    arrowModel->alphaTransparency = 0.5f;
}
void initDummy() {
    dummy = new cMesh();
    dummy->meshName = "HockeyPlayer.ply";
    dummy->scale = 0.2f;
    dummy->positionXYZ = ::g_pConfigManager->_shotPositions[0] + ::g_shotInfo.playerOffset;
    dummy->textureNames[0] = "Player_SharedColors.bmp";
    dummy->textureRatios[0] = 1.f;
    dummy->alphaTransparency = ::g_shotInfo.playerTransparency;
    dummy->wholeObjectShininess_SpecPower = 500000.f;


    //{
    //    "Model":"HockeyPlayer.ply",
    //        "Scale" : 0.2,
    //        "SpecPower" : 500000,
    //        "Texture0" : "Player_SharedColors.bmp",
    //        "MaskTexture0" : "Player_JerseyMask.bmp",
    //        "MaskTextureFill0_Options" : ["Red.bmp"] ,
    //        "MaskTexture1" : "Player_SkinMask.bmp",
    //        "MaskTextureFill1_Options" : ["skintone00.bmp", "skintone01.bmp", "skintone02.bmp", "skintone03.bmp", "skintone04.bmp"] ,
    //        "SpecularMapTexture" : "Player_SpecularMap.bmp"
    //}
}
void ArrowMotion() {
    float shotAdjSpeed = 0.01f;
    if (keys[GLFW_KEY_LEFT]) {
        if ((arrowModel->orientationXYZ.y + shotAdjSpeed) * -1 >= (g_shotInfo.lowerYaw)) {
            arrowModel->orientationXYZ += glm::vec3(0.f, shotAdjSpeed, 0.f);
        }
    }
    if (keys[GLFW_KEY_RIGHT]) {
        if ((arrowModel->orientationXYZ.y - shotAdjSpeed) * -1 <= (g_shotInfo.upperYaw)) {
            arrowModel->orientationXYZ -= glm::vec3(0.f, shotAdjSpeed, 0.f);
        }
    }
    if (keys[GLFW_KEY_UP]) {
        if ((arrowModel->orientationXYZ.x - shotAdjSpeed) * -1 <= (g_shotInfo.upperPitch)) {
            arrowModel->orientationXYZ -= glm::vec3(shotAdjSpeed, 0.f, 0.f);
        }
    }
    if (keys[GLFW_KEY_DOWN]) {
        if ((arrowModel->orientationXYZ.x + shotAdjSpeed) * -1 >= (g_shotInfo.lowerPitch)) {
            arrowModel->orientationXYZ += glm::vec3(shotAdjSpeed, 0.f, 0.f);
        }
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
    ::g_TitleText = "Curtis Tremblay - INFO-6019/6025 Final";
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

    ::g_pFlyCamera->setEye(::g_pConfigManager->_shotPositions[0] + ::g_pConfigManager->_cameraOffset);
    // Eventually figure out how to adjust orientation of camera so it doesn't 'jump' on first movement.
    //::g_pFlyCamera->setAt(::g_pConfigManager->_cameraStartingOrientation);

    worldSpace->Instance()->SetWorldBounds(::g_pConfigManager->_positiveBounds, ::g_pConfigManager->_negativeBounds, ::g_pConfigManager->_cornerRadius, ::g_pConfigManager->_samplePoints);

    cShaderManager::cShader vertShader;
    cShaderManager::cShader fragShader;
    InitShaders(program, vertShader, fragShader);

    glUseProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    GLint matModel_Location = glGetUniformLocation(program, "matModel");
    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
    GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");
    #pragma endregion

    #pragma region Lights
    ::g_pTheLights->theLights[0].position = glm::vec4(::g_pConfigManager->_homeGoalLightPosition, 1.f);
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(1.f, 0.f, 0.f, 1.f);
    ::g_pTheLights->theLights[0].specular = glm::vec4(1.f, 0.f, 0.f, 1.f);
    ::g_pTheLights->theLights[0].atten = glm::vec4(0.003f, 0.0001f, 0.00027f, 20.f);
    ::g_pTheLights->theLights[0].param1.x = 0.f;    // point light
    ::g_pTheLights->TurnOffLight(0);

    ::g_pTheLights->theLights[1].position = glm::vec4(::g_pConfigManager->_awayGoalLightPosition, 1.f);
    ::g_pTheLights->theLights[1].diffuse = glm::vec4(1.f, 0.f, 0.f, 1.f);
    ::g_pTheLights->theLights[1].specular = glm::vec4(1.f, 0.f, 0.f, 1.f);
    ::g_pTheLights->theLights[1].atten = glm::vec4(0.003f, 0.0001f, 0.00027f, 20.f);
    ::g_pTheLights->theLights[1].param1.x = 0.f;    // point light
    ::g_pTheLights->TurnOffLight(1);

    ::g_pConfigManager->setupLights(::g_pTheLights, 2);

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

    //Setup physics boundary visualizers
    ::g_vec_pBoundaries.push_back(new cMesh());
    ::g_vec_pBoundaries[0]->meshName = "tile.ply";
    ::g_vec_pBoundaries[0]->bIsWireframe = true;
    ::g_vec_pBoundaries[0]->bUseObjectDebugColour = true;
    ::g_vec_pBoundaries[0]->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    ::g_vec_pBoundaries[0]->bDontLight = true;
    ::g_vec_pBoundaries[0]->scale = 20.0f;
    ::g_vec_pBoundaries[0]->positionXYZ = glm::vec3(::g_pConfigManager->_positiveBounds.x, 0.f, 0.f);
    ::g_vec_pBoundaries[0]->orientationXYZ = glm::vec3(0.f, 0.f, glm::pi<float>() / 2.f);

    ::g_vec_pBoundaries.push_back(new cMesh());
    ::g_vec_pBoundaries[1]->meshName = "tile.ply";
    ::g_vec_pBoundaries[1]->bIsWireframe = true;
    ::g_vec_pBoundaries[1]->bUseObjectDebugColour = true;
    ::g_vec_pBoundaries[1]->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    ::g_vec_pBoundaries[1]->bDontLight = true;
    ::g_vec_pBoundaries[1]->scale = 20.0f;
    ::g_vec_pBoundaries[1]->positionXYZ = glm::vec3(::g_pConfigManager->_negativeBounds.x, 0.f, 0.f);
    ::g_vec_pBoundaries[1]->orientationXYZ = glm::vec3(0.f, 0.f, glm::pi<float>() / 2.f);

    ::g_vec_pBoundaries.push_back(new cMesh());
    ::g_vec_pBoundaries[2]->meshName = "tile.ply";
    ::g_vec_pBoundaries[2]->bIsWireframe = true;
    ::g_vec_pBoundaries[2]->bUseObjectDebugColour = true;
    ::g_vec_pBoundaries[2]->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    ::g_vec_pBoundaries[2]->bDontLight = true;
    ::g_vec_pBoundaries[2]->scale = 20.0f;
    ::g_vec_pBoundaries[2]->positionXYZ = glm::vec3(0.f, 0.f, ::g_pConfigManager->_positiveBounds.z);
    ::g_vec_pBoundaries[2]->orientationXYZ = glm::vec3(glm::pi<float>() / 2.f, 0.f, 0.f);

    ::g_vec_pBoundaries.push_back(new cMesh());
    ::g_vec_pBoundaries[3]->meshName = "tile.ply";
    ::g_vec_pBoundaries[3]->bIsWireframe = true;
    ::g_vec_pBoundaries[3]->bUseObjectDebugColour = true;
    ::g_vec_pBoundaries[3]->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    ::g_vec_pBoundaries[3]->bDontLight = true;
    ::g_vec_pBoundaries[3]->scale = 20.0f;
    ::g_vec_pBoundaries[3]->positionXYZ = glm::vec3(0.f, 0.f, ::g_pConfigManager->_negativeBounds.z);
    ::g_vec_pBoundaries[3]->orientationXYZ = glm::vec3(glm::pi<float>() / 2.f, 0.f, 0.f);

    ::g_vec_pBoundaries.push_back(new cMesh());
    ::g_vec_pBoundaries[4]->meshName = "tile.ply";
    ::g_vec_pBoundaries[4]->bIsWireframe = true;
    ::g_vec_pBoundaries[4]->bUseObjectDebugColour = true;
    ::g_vec_pBoundaries[4]->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    ::g_vec_pBoundaries[4]->bDontLight = true;
    ::g_vec_pBoundaries[4]->scale = 20.0f;
    ::g_vec_pBoundaries[4]->positionXYZ = glm::vec3(0.f, ::g_pConfigManager->_positiveBounds.y, 0.f);

    ::g_vec_pBoundaries.push_back(new cMesh());
    ::g_vec_pBoundaries[5]->meshName = "tile.ply";
    ::g_vec_pBoundaries[5]->bIsWireframe = true;
    ::g_vec_pBoundaries[5]->bUseObjectDebugColour = true;
    ::g_vec_pBoundaries[5]->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    ::g_vec_pBoundaries[5]->bDontLight = true;
    ::g_vec_pBoundaries[5]->scale = 20.0f;
    ::g_vec_pBoundaries[5]->positionXYZ = glm::vec3(0.f, ::g_pConfigManager->_negativeBounds.y, 0.f);

    #endif

    #pragma endregion

    #pragma region Models
    ::g_pVAOManager->setFilePath("assets/models/");
    std::vector<std::string> vecModelsToLoad;
    vecModelsToLoad.push_back("Sphere_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("ISO_Shphere_flat_3div_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
    vecModelsToLoad.push_back("Quad_x3_2_sided_axial_imposter_base_on_XY_axis.ply");
    vecModelsToLoad.push_back("tile.ply");
    vecModelsToLoad.push_back("puck_normalized.ply");
    vecModelsToLoad.push_back("HockeyPlayer.ply");

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
    ::g_pTextureManager->Create2DTextureFromBMPFile("arrow.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Player_JerseyMask.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Player_SharedColors.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Player_SkinMask.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Player_SpecularMap.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("blue.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("skintone03.bmp", true);

    for (std::string texture : ::g_pConfigManager->_texturesToLoad) {
        ::g_pTextureManager->Create2DTextureFromBMPFile(texture, true);
    }

    // Add a skybox texture
    std::string errorTextString;
    ::g_pTextureManager->SetBasePath("assets/textures/cubemaps");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("WinterRiver",
        "winterRiver_posX.bmp",    /* posX_fileName */
        "winterRiver_negX.bmp",     /*negX_fileName */
        "winterRiver2_negY.bmp",     /*negY_fileName*/
        "winterRiver_posY.bmp",       /*posY_fileName*/
        "winterRiver2_posZ.bmp",    /*posZ_fileName*/
        "winterRiver2_negZ.bmp",      /*negZ_fileName*/
        true, errorTextString))
    {
        std::cout << "Didn't load because: " << errorTextString << std::endl;
    }
    else
    {
        std::cout << "Loaded the skybox cube texture OK" << std::endl;
    }
    #pragma endregion

#pragma region Objects
    ::g_pConfigManager->loadModelsIntoVAO(program, *::g_pVAOManager, true);

    ::g_vec_pMeshes = ::g_pConfigManager->_rink;

    initDummy();
    initArrow();
    ::g_vec_pMeshes.push_back(arrowModel);
    ::g_vec_pMeshes.push_back(dummy);
    worldSpace->SetupNet(glm::vec3(0.f, 5.f, 216.f));

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

    //::g_vec_Entities = ::g_pConfigManager->actorEntities;


#pragma endregion
    const double MAX_DELTA_TIME = 0.025;  // 50 ms
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
        ArrowMotion();

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

        for (iEntity* actor : ::g_vec_Entities) {
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
        if (::g_bShowCollisionObjects) {
            for (unsigned int index = 0; index != ::g_vec_pBoundaries.size(); index++)
            {
                // So the code is a little easier...
                cMesh* pCurrentMesh = ::g_vec_pBoundaries[index];

                matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

                DrawObject(pCurrentMesh,
                    matModel,
                    matModel_Location,
                    matModelInverseTranspose_Location,
                    program,
                    ::g_pVAOManager);
            }

            for (unsigned int index = 0; index != ::g_vec_Entities.size(); index++)
            {
                // So the code is a little easier...
                cMesh* pCurrentMesh = ::g_vec_Entities[index]->GetDebugMesh();

                matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

                DrawObject(pCurrentMesh,
                    matModel,
                    matModel_Location,
                    matModelInverseTranspose_Location,
                    program,
                    ::g_pVAOManager);
            }
        }

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
                // Find closest vertex for both objects. If this is identified as a performance bottleneck, optimize by only looking up/tracking exact vertex info if requested.
                // Otherwise, just use position to get an approximation of where the object is located.
                // This may also fail for more complex geometry, or cause other issues: https://www.khronos.org/opengl/wiki/Transparency_Sorting
                float minDistanceA = -1;     // Default to -1 so any valid distance will replace this.
                float minDistanceB = -1;     // Default to -1 so any valid distance will replace this.
                float candidate = 0;
                for (const sVertex& vertex : ::g_pConfigManager->_vertexData[a->meshName]) {
                    candidate = glm::distance((a->positionXYZ + glm::vec3(vertex.x, vertex.y, vertex.z)), ::g_pFlyCamera->getEye());
                    if (candidate < minDistanceA || minDistanceA < 0) {
                        minDistanceA = candidate;
                    }
                }
                for (const sVertex& vertex : ::g_pConfigManager->_vertexData[b->meshName]) {
                    candidate = glm::distance((b->positionXYZ + glm::vec3(vertex.x, vertex.y, vertex.z)), ::g_pFlyCamera->getEye());
                    if (candidate < minDistanceB || minDistanceB < 0) {
                        minDistanceB = candidate;
                    }
                }
                return minDistanceA > minDistanceB;
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