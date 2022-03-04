#pragma once

#include <iostream>
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp> // glm::vec3
#include <extern/glm/vec4.hpp> // glm::vec4
#include <extern/glm/mat4x4.hpp> // glm::mat4
#include <extern/glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <extern/glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "cFBO/cFBO.h"
#include "globals.h"
#include <algorithm>
#include <Physics/cWorldSpace.h>

cWorldSpace* worldSpace = cWorldSpace::Instance();

cMesh* pSkybox;

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

void DrawAllObjects(
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLint matProjection_Location,
    GLuint program,
    bool drawDebug = true);

void DrawAllObjects(
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLint matProjection_Location,
    GLuint program,
    glm::mat4 matView,
    bool drawDebug = true);

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

void ClearFBO(cFBO* fbo) {

    // Set the output of the renderer to the fbo
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);

    // Set the viewport to the size of my offscreen texture (FBO)
    glViewport(0, 0, fbo->width, fbo->height);

    // Turn on the depth buffer
    glEnable(GL_DEPTH);         // Turns on the depth buffer
    glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

    fbo->clearBuffers(true, true);

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
    ::g_TitleText = "Curtis Tremblay - INFO-6020 Midterm";
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

    GLint max_vertex_uniform_components = 0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &max_vertex_uniform_components);
    std::cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS: " << max_vertex_uniform_components << std::endl;

    GLint max_fragment_uniform_components = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &max_fragment_uniform_components);
    std::cout << "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS: " << max_fragment_uniform_components << std::endl;

    GLint max_compute_uniform_components = 0;
    glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &max_compute_uniform_components);
    std::cout << "GL_MAX_COMPUTE_UNIFORM_COMPONENTS: " << max_compute_uniform_components << std::endl;

    GLint max_geometry_uniform_components = 0;
    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &max_geometry_uniform_components);
    std::cout << "GL_MAX_GEOMETRY_UNIFORM_COMPONENTS: " << max_geometry_uniform_components << std::endl;

    ::g_StartUp(pWindow);

    ::g_pFlyCamera->setEye(::g_pConfigManager->_cameraStartingPosition);
    ::g_pTVCamera->setEye(::g_pConfigManager->_cameraStartingPosition + glm::vec3(0.f,0.f,100.f));
    // Eventually figure out how to adjust orientation of camera so it doesn't 'jump' on first movement.
    //::g_pFlyCamera->setAt(::g_pConfigManager->_cameraStartingOrientation);

    worldSpace->Instance()->SetWorldBounds(::g_pConfigManager->_positiveBounds, ::g_pConfigManager->_negativeBounds);

    cShaderManager::cShader vertShader;
    cShaderManager::cShader fragShader;
    cShaderManager::cShader geomShader;
    InitShaders(program, vertShader, geomShader, fragShader);

    glUseProgram(program);
    cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");

    int theUniformIDLoc = -1;
    theUniformIDLoc = glGetUniformLocation(program, "matModel");
    pShaderProc->mapUniformName_to_UniformLocation["matModel"] = theUniformIDLoc;

    // Or...
    pShaderProc->mapUniformName_to_UniformLocation["matModel"] = glGetUniformLocation(program, "matModel");
    pShaderProc->mapUniformName_to_UniformLocation["matView"] = glGetUniformLocation(program, "matView");
    pShaderProc->mapUniformName_to_UniformLocation["matProjection"] = glGetUniformLocation(program, "matProjection");
    pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"] = glGetUniformLocation(program, "matModelInverseTranspose");

    pShaderProc->mapUniformName_to_UniformLocation["wholeObjectSpecularColour"] = glGetUniformLocation(program, "wholeObjectSpecularColour");
    // .. and so on...
    pShaderProc->mapUniformName_to_UniformLocation["theLights[0].position"] = glGetUniformLocation(program, "wholeObjectSpecularColour");
    // ... and so on..
    // ************************************************

    mvp_location = glGetUniformLocation(program, "MVP");
    GLint matModel_Location = glGetUniformLocation(program, "matModel");
    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
    GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");
    #pragma endregion

    #pragma region Lights
    //::g_pTheLights->theLights[0].position = glm::vec4(::g_pConfigManager->_homeGoalLightPosition, 1.f);
    //::g_pTheLights->theLights[0].diffuse = glm::vec4(1.f, 0.f, 0.f, 1.f);
    //::g_pTheLights->theLights[0].specular = glm::vec4(1.f, 0.f, 0.f, 1.f);
    //::g_pTheLights->theLights[0].atten = glm::vec4(0.003f, 0.0001f, 0.00027f, 20.f);
    //::g_pTheLights->theLights[0].param1.x = 0.f;    // point light
    //::g_pTheLights->TurnOffLight(0);

    //::g_pTheLights->theLights[1].position = glm::vec4(glm::vec3(0.f,0.f,-110.f), 1.f);
    //::g_pTheLights->theLights[1].diffuse = glm::vec4(1.f, 0.f, 0.f, 1.f);
    //::g_pTheLights->theLights[1].specular = glm::vec4(1.f, 0.f, 0.f, 1.f);
    //::g_pTheLights->theLights[1].atten = glm::vec4(0.003f, 0.0001f, 0.00027f, 20.f);
    //::g_pTheLights->theLights[1].param1.x = 0.f;    // point light
    //::g_pTheLights->TurnOnLight(1);

    ::g_pConfigManager->setupLights(::g_pTheLights, 0);

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
    vecModelsToLoad.push_back("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply");
    vecModelsToLoad.push_back("tile.ply");
    vecModelsToLoad.push_back("SM_Env_Consoles_01_screen_1_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Consoles_01_screen_3_xyz_n_rgba_uv.ply");

    unsigned int totalVerticesLoaded = 0;
    unsigned int totalTrianglesLoaded = 0;
    for (std::vector<std::string>::iterator itModel = vecModelsToLoad.begin(); itModel != vecModelsToLoad.end(); itModel++)
    {
        sModelDrawInfo theModel;
        std::string modelName = *itModel;
        std::cout << "Loading " << modelName << "...";
        if (!::g_pVAOManager->LoadModelIntoVAO(modelName, theModel, program))
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

    ::g_pEffectsManager->Initialize();
    #pragma endregion

#pragma region Objects
    ::g_pConfigManager->loadModelsIntoVAO(program, *::g_pVAOManager, true);

    ::g_vec_pMeshes = ::g_pConfigManager->_modelsFromConfig;

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
    pSkybox = new cMesh();

    // Mimics a skybox
    pSkybox->meshName = "Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply";
    pSkybox->scale = 5'000'000.0f;

    pSkybox->positionXYZ = ::g_pFlyCamera->getEye();

    ::g_vec_pActors = ::g_pConfigManager->actorEntities;

    cMesh* monitor = new cMesh;
    monitor->meshName = "SM_Env_Consoles_01_screen_1_xyz_n_rgba_uv.ply";
    monitor->friendlyName = "Console Monitor 3";

    monitor->positionXYZ = glm::vec3(0.f, 0.f, 0.f);
    monitor->orientationXYZ = glm::vec3(0.0f, glm::pi<float>()/2, 0.f);
    monitor->scale = 4.2f;
    monitor->bIsWireframe = false;
    monitor->bDontLight = true;
    monitor->bUseObjectDebugColour = true;
    monitor->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);

    //::g_vec_pMeshes.push_back(monitor);

#pragma endregion

#pragma region FBO/2nd Pass Rendering
    // Create the FBO (Frame Buffer Object)
// The texture we can render to
    ::g_pFBO = new cFBO();
    // Set this off screen texture buffer to some random size
    std::string FBOerrorString;
    //    if (::g_pFBO->init(1024, 1024, FBOerrorString))
    if (::g_pFBO->init(1200, 640, FBOerrorString))
    {
        std::cout << "Full Screen FBO is all set!" << std::endl;
    }
    else
    {
        std::cout << "FBO Error: " << FBOerrorString << std::endl;
    }
    cFBO smallFBO;
    // Set this off screen texture buffer to a smaller size since it is being rendered at a low resolution.
    if (smallFBO.init(300, 160, FBOerrorString))
    {
        std::cout << "Smaller FBO is all set!" << std::endl;
    }
    else
    {
        std::cout << "FBO Error: " << FBOerrorString << std::endl;
    }

    // Clear the Original back buffer once, BEFORE we render anything
    float ratio;
    int width, height;
    glfwGetFramebufferSize(pWindow, &width, &height);
    ratio = width / (float)height;
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 164.0f / 255.0f, 239.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const GLint RENDER_PASS_0_ENTIRE_SCENE = 0;
    const GLint RENDER_PASS_1_QUAD_ONLY = 1;
    const GLint PASS_2_MONITOR = 2;
    const GLint PASS_3_2D_EFFECTS_PASS = 3;
    GLint renderPassNumber_LocID = glGetUniformLocation(program, "renderPassNumber");
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

        // Update the title text
        glfwSetWindowTitle(pWindow, ::g_TitleText.c_str());

        // Copy the light information into the shader to draw the scene
        ::g_pTheLights->CopyLightInfoToShader();

#if defined _DEBUG
        ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
#endif

        //worldSpace->_world->Update(deltaTime);

        // Now handle the proper scene.
        ClearFBO(::g_pFBO);

        //glm::vec3 eye = ::g_pActiveCamera->getEye();
        // First get a smaller version of the scene for the monitor.
        {
            ClearFBO(&smallFBO);

            glUniform1ui(renderPassNumber_LocID, RENDER_PASS_0_ENTIRE_SCENE);

            glm::mat4 matView;

            glm::vec3 cameraEye = ::g_pTVCamera->getEye();
            glm::vec3 cameraAt = ::g_pTVCamera->getAtInWorldSpace();
            glm::vec3 cameraUp = ::g_pTVCamera->getUpVector();

            matView = glm::mat4(1.0f);
            matView = glm::lookAt(cameraEye,   // "eye"
                cameraAt,    // "at"
                cameraUp);

            //::g_pActiveCamera->setEye(glm::vec3(0.f, 19.f, 0.f));
            DrawAllObjects(matModel_Location, matModelInverseTranspose_Location, matProjection_Location, program, matView, false);

            //::g_pActiveCamera = ::g_pTVCamera;
            glUniform1ui(renderPassNumber_LocID, PASS_2_MONITOR);
            // Set the FBO colour texture to be the texture source for this quad

            cameraEye = ::g_pActiveCamera->getEye();
            cameraAt = ::g_pActiveCamera->getAtInWorldSpace();
            cameraUp = ::g_pActiveCamera->getUpVector();

            matView = glm::mat4(1.0f);
            matView = glm::lookAt(cameraEye,   // "eye"
                cameraAt,    // "at"
                cameraUp);

            glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
                1, GL_FALSE, glm::value_ptr(matView));

            GLuint FSQ_textureUnit = 8;	    // We picked 8 just because it's close to our arbitrary 7 value.
            glActiveTexture(FSQ_textureUnit + GL_TEXTURE0);
            GLuint TextureNumber = smallFBO.colourTexture_0_ID;
            glBindTexture(GL_TEXTURE_2D, TextureNumber);

            // uniform sampler2D texture_08;
            GLint FSQ_textureSamplerID = glGetUniformLocation(program, "texture_08");
            glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);

            glCullFace(GL_FRONT);

            //::g_pActiveCamera->setEye(eye);
            // Set pass to #0
            glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBO->ID);
            glViewport(0, 0, ::g_pFBO->width, ::g_pFBO->height);

            //::g_pActiveCamera->setEye(eye);
            //::g_pActiveCamera->setAt(at);

            for (unsigned int index = 0; index != ::g_vec_pMonitorsQ4.size(); index++)
            {
                // So the code is a little easier...
                cMesh* pCurrentMesh = ::g_vec_pMonitorsQ4[index];

                matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

                DrawObject(pCurrentMesh,
                    glm::mat4(1.0f),
                    matModel_Location,
                    matModelInverseTranspose_Location,
                    program,
                    ::g_pVAOManager);
            }
        }

        ::g_pFlyCamera->Update(deltaTime);
        glUniform1ui(renderPassNumber_LocID, RENDER_PASS_0_ENTIRE_SCENE);

        // *******************************************************
        // Screen is cleared and we are ready to draw the scene...
        // *******************************************************

        DrawAllObjects(matModel_Location, matModelInverseTranspose_Location, matProjection_Location, program);

        // Now that the scene has been rendered to the FBO, Get it's colour texture and write to our full screen quad.
        if (::g_pFullScreenQuad == NULL)
        {
            ::g_pFullScreenQuad = new cMesh;
            ::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply";
            ::g_pFullScreenQuad->friendlyName = "Full_Screen_Quad";

            ::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 500.0f);
            ::g_pFullScreenQuad->scale = 100.0f;
            ::g_pFullScreenQuad->bIsWireframe = false;
            ::g_pFullScreenQuad->bDontLight = true;
            ::g_pFullScreenQuad->bUseObjectDebugColour = true;
            ::g_pFullScreenQuad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        }
        // Point the output of the renderer to the real framebuffer

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //  Clear the frame buffer. 
        // NOTE: Using the Microsoft DirectX light blue colour from here, but can be another colour:
        // https://usbrandcolors.com/microsoft-colors/
        glClearColor(0.0f, 164.0f / 255.0f, 239.0f / 255.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(pWindow, &width, &height);
        ratio = width / (float)height;

        matProjection = glm::perspective(
            ::g_pActiveCamera->FOV,
            ratio,
            ::g_pActiveCamera->nearPlane,      // Near plane (as large as possible)
            ::g_pActiveCamera->farPlane);      // Far plane (as small as possible)

        glViewport(0, 0, width, height);

        GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
        glUniform2f(screenWidthHeight_locID, width, height);

        glUniform1ui(renderPassNumber_LocID, PASS_3_2D_EFFECTS_PASS);

        // Set the FBO colour texture to be the texture source for this quad

        GLuint FSQ_textureUnit = 7;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
        glActiveTexture(FSQ_textureUnit + GL_TEXTURE0);
        GLuint TextureNumber = ::g_pFBO->colourTexture_0_ID;
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // uniform sampler2D texture_07;
        GLint FSQ_textureSamplerID = glGetUniformLocation(program, "texture_07");
        glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);

        glm::mat4x4 matModelFullScreenQuad = glm::mat4(1.0f);   // identity matrix

        glCullFace(GL_FRONT);

        // Place the camera in front of the quad (the "full screen" quad)
        // Quad location is ::g_pFullScreenQuad->positionXYZ = glm::vec3( 0.0f, 0.0f, 500.0f);

        matView = glm::mat4(1.0f);

        glm::vec3 eyeForFullScreenQuad = glm::vec3(0.0f, 0.0f, 450.0f);   // "eye" is 100 units away from the quad
        glm::vec3 atForFullScreenQuad = glm::vec3(0.0f, 0.0f, 500.0f);    // "at" the quad
        glm::vec3 upForFullScreenQuad = glm::vec3(0.0f, 1.0f, 0.0f);      // "at" the quad
        matView = glm::lookAt(eyeForFullScreenQuad,
            atForFullScreenQuad,
            upForFullScreenQuad);      // up in y direction
	
        matView = glm::ortho(
            0.0f,   // Left
            1.0f / (float)width,  // Right
            0.0f,   // Top
            1.0f / (float)height, // Bottom
            30.0f, // zNear  Eye is at 450, quad is at 500, so 50 units away
            70.0f); // zFar

        glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
            1, GL_FALSE, glm::value_ptr(matView));

        DrawObject(::g_pFullScreenQuad,
            matModelFullScreenQuad,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);

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
        glm::vec3 eye = ::g_pActiveCamera->getEye();
        glm::vec3 at = ::g_pActiveCamera->getAtInWorldSpace();

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

void DrawAllObjects(
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLint matProjection_Location,
    GLuint program,
    glm::mat4 matView,
    bool drawDebug ) {

    glm::mat4 matModel;             // used to be "m"; Sometimes it's called "world"
    glm::mat4 matProjection;        // used to be "p";
    float ratio = ::g_pFBO->width / (float)::g_pFBO->height;

    matProjection = glm::perspective(
        ::g_pActiveCamera->FOV,
        ratio,
        ::g_pActiveCamera->nearPlane,      // Near plane (as large as possible)
        ::g_pActiveCamera->farPlane);      // Far plane (as small as possible)

    cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");

    glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
        1, GL_FALSE, glm::value_ptr(matView));


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
    if (drawDebug) {

        DrawDebugObjects(matModel_Location, matModelInverseTranspose_Location, program, ::g_pVAOManager);

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

            for (unsigned int index = 0; index != ::g_vec_pActors.size(); index++)
            {
                // So the code is a little easier...
                cMesh* pCurrentMesh = ::g_vec_pActors[index]->GetDebugMesh();

                matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

                DrawObject(pCurrentMesh,
                    matModel,
                    matModel_Location,
                    matModelInverseTranspose_Location,
                    program,
                    ::g_pVAOManager);
            }
        }
    }
#endif

    // After drawing the other objects, draw the skybox to limit overdraw.

    GLint bIsSkyBox_LocID = glGetUniformLocation(program, "bIsSkyBox");
    glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);

    // Move the "skybox object" with the camera
    pSkybox->positionXYZ = ::g_pActiveCamera->getEye();
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
            //float minDistanceA = -1;     // Default to -1 so any valid distance will replace this.
            //float minDistanceB = -1;     // Default to -1 so any valid distance will replace this.
            //float candidate = 0;
            //for (const sVertex& vertex : ::g_pConfigManager->_vertexData[a->meshName]) {
            //    candidate = glm::distance((a->positionXYZ + glm::vec3(vertex.x, vertex.y, vertex.z)), ::g_pFlyCamera->getEye());
            //    if (candidate < minDistanceA || minDistanceA < 0) {
            //        minDistanceA = candidate;
            //    }
            //}
            //for (const sVertex& vertex : ::g_pConfigManager->_vertexData[b->meshName]) {
            //    candidate = glm::distance((b->positionXYZ + glm::vec3(vertex.x, vertex.y, vertex.z)), ::g_pFlyCamera->getEye());
            //    if (candidate < minDistanceB || minDistanceB < 0) {
            //        minDistanceB = candidate;
            //    }
            //}
            return glm::distance(a->positionXYZ, ::g_pActiveCamera->getEye()) > glm::distance(b->positionXYZ, ::g_pActiveCamera->getEye());
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
}

void DrawAllObjects(
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLint matProjection_Location,
    GLuint program,
    bool drawDebug) 
{
    glm::mat4 matView;

    glm::vec3 cameraEye = ::g_pActiveCamera->getEye();
    glm::vec3 cameraAt = ::g_pActiveCamera->getAtInWorldSpace();
    glm::vec3 cameraUp = ::g_pActiveCamera->getUpVector();

    matView = glm::mat4(1.0f);
    matView = glm::lookAt(cameraEye,   // "eye"
        cameraAt,    // "at"
        cameraUp);

    DrawAllObjects(matModel_Location, matModelInverseTranspose_Location, matProjection_Location, program, matView, drawDebug);
}