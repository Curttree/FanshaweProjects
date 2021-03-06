#pragma once

#include <iostream>
#include <limits>
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp> // glm::vec3
#include <extern/glm/vec4.hpp> // glm::vec4
#include <extern/glm/mat4x4.hpp> // glm::mat4
#include <extern/glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <extern/glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <Graphics/cFrustumCullingHandler.h>

#include "globals.h"
#include "DecorationManager.h"

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

int main(void) {
    //Seed rand.
    srand(time(NULL));
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

    pWindow = glfwCreateWindow(1200, 640, "'Something's Fishy' | Curtis Tremblay - Fanshawe 2022 Game Jam Project", NULL, NULL);

    if (!pWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetKeyCallback(pWindow, GLFW_key_callback);
    glfwSetCursorEnterCallback(pWindow, GLFW_cursor_enter_callback);
    glfwSetCursorPosCallback(pWindow, GLFW_cursor_position_callback);
    glfwSetScrollCallback(pWindow, GLFW_scroll_callback);
    glfwSetMouseButtonCallback(pWindow, GLFW_mouse_button_callback);
    glfwSetWindowSizeCallback(pWindow, GLFW_window_size_callback);

    glfwMakeContextCurrent(pWindow);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    ::g_pWindow = pWindow;
    
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

    ::g_pGameEngine->Initialize();

    cShaderManager::cShader vertShader;
    cShaderManager::cShader fragShader;
    cShaderManager::cShader geomShader;
    InitShaders(program, vertShader, geomShader, fragShader);

    glUseProgram(program);

    // ************************************************
    // Now, I'm going to "load up" all the uniform locations
    // (This was to show how a map could be used)
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

    ::g_pTheLights->theLights[0].position = glm::vec4(-5000.0f, 10000.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //... and so on...
//    	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//	                // 0 = pointlight
//					// 1 = spot light
//					// 2 = directional light
//    ::g_pTheLights->theLights[0].param1.x = 1.0f;    // Spot light
    ::g_pTheLights->theLights[0].param1.x = 2.0f;    // Directional light
    ::g_pTheLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(0.5725f, 0.3725f, 0.5882f, 1.0f);
    ::g_pTheLights->theLights[0].param1.y = 15.0f;   // Inner
    ::g_pTheLights->theLights[0].param1.z = 30.0f;   // Outer
    ::g_pTheLights->theLights[0].atten.y = 0.001f;
    ::g_pTheLights->theLights[0].atten.z = 0.00001f;
    ::g_pTheLights->TurnOnLight(0);  // Or this!

    // Get the uniform locations of the light shader values
    ::g_pTheLights->SetUpUniformLocations(program);

    // Set up the debug sphere object
    ::g_pDebugSphere = new cMesh("Sphere_xyz_n_rgba_uv.ply");
    ::g_pDebugSphere->bIsWireframe = true;
    ::g_pDebugSphere->bUseObjectDebugColour = true;
    ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pDebugSphere->bDontLight = true;
    ::g_pDebugSphere->scale = glm::vec3(1.0f);
    ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
    // Give this a friendly name
    ::g_pDebugSphere->friendlyName = "Debug Sphere";
    // Then I could make a small function that searches for that name

    ::g_pVAOManager->setFilePath("assets/models/");
    std::vector<std::string> vecModelsToLoad;
    vecModelsToLoad.push_back("Sphere_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("ISO_Shphere_flat_3div_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply");
    vecModelsToLoad.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
    vecModelsToLoad.push_back("can.ply");
    vecModelsToLoad.push_back("SM_Prop_CarboardBox_02.ply");
    vecModelsToLoad.push_back("ColtPython.ply");


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
    }//for (std::vector<std::string>::iterator itModel


    std::cout << "Done loading models." << std::endl;
    std::cout << "Total vertices loaded = " << totalVerticesLoaded << std::endl;
    std::cout << "Total triangles loaded = " << totalTrianglesLoaded << std::endl;

    // Load the textures
    ::g_pTextureManager->SetBasePath("assets/textures");

    ::g_pTextureManager->Create2DTextureFromBMPFile("BrightColouredUVMap.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("BrainNerve.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("crosshair.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("PolygonCity_Texture2.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("PolygonCity_Road_01.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("city_imposter.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Concrete_012.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("UnderwaterNormals.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Fish_BaseColor.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("BrushedMetalSilver2.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("bubble.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("red.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Male_16_Specular.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Animations/smoke00.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Animations/smoke01.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Animations/smoke02.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Animations/smoke03.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Animations/smoke04.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("mission_title.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("mission_complete.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("can_icon.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("can_icon_x.bmp", true);
    

    // Add a skybox texture
    std::string errorTextString;
    ::g_pTextureManager->SetBasePath("assets/textures/cubemaps");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Skybox",
        "uw_rt.bmp",    /* posX_fileName */
        "uw_lf.bmp",     /*negX_fileName */
        "uw_dn.bmp",     /*negY_fileName*/
        "uw_up.bmp",       /*posY_fileName*/
        "uw_bk.bmp",    /*posZ_fileName*/
        "uw_ft.bmp",      /*negZ_fileName*/
        true, errorTextString))
    {
        std::cout << "Didn't load because: " << errorTextString << std::endl;
    }
    else
    {
        std::cout << "Loaded the skybox cube texture OK" << std::endl;
    }

    const double MAX_DELTA_TIME = 0.1;  // 100 ms
    double previousTime = glfwGetTime();

    // Create the FBO (Frame Buffer Object)
    // The texture we can render to
    ::g_pFBO = new cFBO();
    // Set this off screen texture buffer to some random size
    std::string FBOerrorString;
    if (::g_pFBO->init(1200, 640, FBOerrorString))
    {
        std::cout << "FBO is all set!" << std::endl;
    }
    else
    {
        std::cout << "FBO Error: " << FBOerrorString << std::endl;
    }
    cFBO uiFBO;
    // Set this off screen texture buffer to a smaller size since it is being rendered at a low resolution.
    if (uiFBO.init(1200, 640, FBOerrorString))
    {
        std::cout << "UI FBO is all set!" << std::endl;
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
    const GLint PASS_4_UI_PASS = 4;
    GLint renderPassNumber_LocID = glGetUniformLocation(program, "renderPassNumber");

#pragma region Objects

    // Place cans
    ::g_pGameEngine->g_pGameplayManager->SetupScene();

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
    cMesh* pSkybox = new cMesh("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");

    // Mimics a skybox
    pSkybox->scale = glm::vec3(5'000'000.0f);

    pSkybox->positionXYZ = ::g_pFlyCamera->getEye();

    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Bld_Apartment_Stack_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Bld_Apartment_Stack_02.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Bld_Apartment_Stack_03.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Bld_Apartment_Roof_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Bld_Apartment_Roof_02.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Bld_Apartment_Roof_03.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Bld_Apartment_Door_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Bld_Apartment_Door_02.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Env_Sidewalk_Straight_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Env_Road_YellowLines_02.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Prop_HotdogStand_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Prop_Barrier_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Env_Sidewalk_Corner_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Bld_Apartment_Stairs_Planter_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Env_Sidewalk_Gutter_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Prop_LightPole_Base_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Prop_ParkingMeter_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Prop_ParkBench_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Env_Road_Bare_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Env_Sidewalk_01.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("City/SM_Prop_LightPole_Arm_01.fbx", program);
    
    
   // ::g_pVAOManager->LoadMeshWithAssimp("blobby.fbx", program);
    //::g_pVAOManager->LoadMeshWithAssimp("oldMan.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("Characters/Detective/detective@idle.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("Characters/Detective/detective@aim.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("Characters/Detective/detective@shoot.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("Characters/Detective/detective@wait.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("Characters/Detective/detective@walk.fbx", program);
    ::g_pVAOManager->LoadMeshWithAssimp("Characters/Detective/detective@run.fbx", program);
    

    ::g_pGameEngine->g_pGameplayManager->GameStart();
    DecorationManager::Instance()->DecorateScene();
    ::g_pGameEngine->entityManager.InitCharacter();

    //For debugging animation state transitions.
    std::string oldState = ::g_pGameEngine->entityManager.GetPlayer()->GetAnimationStateAsString();
    ::g_pGameEngine->entityManager.GetPlayer()->LoadAnimation();
    ::g_pGameEngine->entityManager.GetPlayer()->LoadBones();
    std::string currentState;

    ::g_pFlyCamera->MoveUpDown_Y(2.f);

    glm::vec3 uiEye = ::g_pFlyCamera->getEye();
    glm::vec3 uiAt = ::g_pFlyCamera->getAtInWorldSpace();
    glm::vec3 uiUp = ::g_pFlyCamera->getUpVector();

#pragma endregion
    while (!glfwWindowShouldClose(pWindow)) {
        currentState = ::g_pGameEngine->entityManager.GetPlayer()->GetAnimationStateAsString();
        if (oldState != currentState) {
            //std::cout << currentState << std::endl;
            ::g_pGameEngine->entityManager.GetPlayer()->LoadAnimation();
            oldState = currentState;
        }
        // Set pass to #0
        glUniform1ui(renderPassNumber_LocID, RENDER_PASS_0_ENTIRE_SCENE);

        float ratio;
        int width, height;
        glm::mat4 matModel;             // used to be "m"; Sometimes it's called "world"
        glm::mat4 matProjection;        // used to be "p";
        glm::mat4 matView;              // used to be "v";

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
        previousTime = currentTime;
        ::g_deltaTime = deltaTime;
        if (::g_runTime + deltaTime < FLT_MAX) {
            ::g_runTime += deltaTime;
        }

        ::g_pGameEngine->Update(deltaTime);

        // Set the output of the renderer to the screen (default FBO)
        ClearFBO(::g_pFBO);
        ClearFBO(&uiFBO);
        ratio = ::g_pFBO->width / (float)::g_pFBO->height;

        // *******************************************************
        // Screen is cleared and we are ready to draw the scene...
        // *******************************************************

        // Update the title text
        //glfwSetWindowTitle(pWindow, "'Something's Fishy' | Curtis Tremblay - Fanshawe 2022 Game Jam Project");


        // Copy the light information into the shader to draw the scene
        ::g_pTheLights->CopyLightInfoToShader();

        ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
        // Place the "debug sphere" at the same location as the selected light (again)
        // HACK: Debug sphere is 5th item added
//        ::g_vecMeshes[5].positionXYZ = gTheLights.theLights[0].position;

        matProjection = glm::perspective<float>(
            ::g_pFlyCamera->FOV,
            ratio,
            ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
            ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

        //matProjection = glm::perspective(
        //    0.6f,       // in degrees
        //    ratio,
        //    10.0f,       // Near plane (as large as possible)
        //    1'000'000.0f);   // Far plane (as small as possible)

        ::g_pFlyCamera->Update(deltaTime);
        ::g_pGameEngine->entityManager.TimeStep(deltaTime);


        matModel = glm::mat4(1.0f);


        cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");



        glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));

        sFrustum frustum = cFrustumCullingHandler::Instance()->createFromCamera(::g_pFlyCamera);

        matView = glm::mat4(1.0f);
        matView = glm::lookAt(uiEye,   // "eye"
            uiAt,    // "at"
            uiUp);

        glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
            1, GL_FALSE, glm::value_ptr(matView));

        std::vector<cEntity*> ui_entities = ::g_pGameEngine->entityManager.GetUIEntities();
        for (unsigned int index = 0; index != ui_entities.size(); index++)
        {
            // So the code is a little easier...
            cMesh* pCurrentMesh = ui_entities[index]->mesh;

            DrawObject(pCurrentMesh,
                matModel,
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                ::g_pVAOManager);
        }
        {
            GLuint FSQ_textureUnit = 33;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
            glActiveTexture(FSQ_textureUnit + GL_TEXTURE0);
            GLuint TextureNumber = uiFBO.colourTexture_0_ID;
            glBindTexture(GL_TEXTURE_2D, TextureNumber);

            // uniform sampler2D texture_07;
            GLint FSQ_textureSamplerID = glGetUniformLocation(program, "uiTexture");
            glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);
        }

        glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
        glm::vec3 cameraAt = ::g_pFlyCamera->getAtInWorldSpace();
        glm::vec3 cameraUp = ::g_pFlyCamera->getUpVector();

        glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBO->ID);
        glViewport(0, 0, ::g_pFBO->width, ::g_pFBO->height);
        matView = glm::mat4(1.0f);
        matView = glm::lookAt(cameraEye,   // "eye"
            cameraAt,    // "at"
            cameraUp);

        glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
            1, GL_FALSE, glm::value_ptr(matView));
        // **********************************************************************
        // Draw the "scene" of all objects.
        // i.e. go through the vector and draw each one...
        // **********************************************************************

        for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
        {
            // So the code is a little easier...
            cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

            if (!cFrustumCullingHandler::Instance()->isWithinFrustum(frustum, pCurrentMesh)) {
                //Object isn't in view. Don't bother drawing.
                continue;
            }

            DrawObject(pCurrentMesh,
                matModel,
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                ::g_pVAOManager);
        }//for (unsigned int index
        std::vector<cEntity*> entities = ::g_pGameEngine->entityManager.GetEntities();
        for (unsigned int index = 0; index != entities.size(); index++)
        {
            // So the code is a little easier...
            cMesh* pCurrentMesh = entities[index]->mesh;

            if (!cFrustumCullingHandler::Instance()->isWithinFrustum(frustum, pCurrentMesh)) {
                //Object isn't in view. Don't bother drawing.
                continue;
            }

            DrawObject(pCurrentMesh,
                matModel,
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                ::g_pVAOManager);
        }
        cMesh* playerMesh = ::g_pGameEngine->entityManager.GetPlayer()->mesh;
        DrawObject(playerMesh,
            matModel,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);

        //Uncomment if you want to show the physics approximation of the character.
        //cMesh* proxyMesh = ::g_pGameEngine->entityManager.GetPlayer()->physicsProxy->mesh;
        //DrawObject(proxyMesh,
        //    matModel,
        //    matModel_Location,
        //    matModelInverseTranspose_Location,
        //    program,
        //    ::g_pVAOManager);

        std::vector<cParticle*> particles = ::g_pGameEngine->entityManager.GetParticles();
        for (unsigned int index = 0; index != particles.size(); index++)
        {
            cMesh* pCurrentMesh = particles[index]->mesh;

            //if (!cFrustumCullingHandler::Instance()->isWithinFrustum(frustum, pCurrentMesh)) {
            //    //Object isn't in view. Don't bother drawing.
            //    continue;
            //}

            //TODO: After they are culled, update sorting so we aren't overdrawing particles as regularly.


            DrawObject(pCurrentMesh,
                matModel,
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                ::g_pVAOManager);
        }
        // Scene is drawn
        // **********************************************************************   

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


        //DrawDebugObjects(matModel_Location, matModelInverseTranspose_Location, program, ::g_pVAOManager);

        //TODO: Replace with own implementation
        // 2nd pass of the render, where we do something bizzare
        if (::g_pFullScreenQuad == NULL)
        {
            ::g_pFullScreenQuad = new cMesh("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply");
            //            ::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_2_sided_aligned_on_XY_plane.ply";
            //            ::g_pFullScreenQuad->meshName = "bun_zipper_xyz_rgba_uv.ply";
            ::g_pFullScreenQuad->friendlyName = "Full_Screen_Quad";

            // For now, place quad on the right side
            ::g_pFullScreenQuad->positionXYZ = glm::vec3( 0.0f, 0.0f, 500.0f);
            ::g_pFullScreenQuad->scale = glm::vec3(100.0f);
            //            ::g_pFullScreenQuad->bIsWireframe = true;
            ::g_pFullScreenQuad->bIsWireframe = false;
            ::g_pFullScreenQuad->bDontLight = true;
            ::g_pFullScreenQuad->bUseObjectDebugColour = true;
            ::g_pFullScreenQuad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        }
        // Point the output of the renderer to the real framebuffer

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //  Clear the frame buffer. 
        // NOTE: I'm clearing the color bit AND the depth buffer
        // I'm using the Microsoft DirectX light blue colour from here:
        // https://usbrandcolors.com/microsoft-colors/
        glClearColor(0.0f, 164.0f / 255.0f, 239.0f / 255.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(pWindow, &width, &height);
        if (height <= 0.f || width <= 0.f) {
            height = 1.f;
        }
        ratio = width / (float)height;

        matProjection = glm::perspective <float> (
            ::g_pFlyCamera->FOV,
            ratio,
            ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
            ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

        glViewport(0, 0, width, height);

        GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
        glUniform2f(screenWidthHeight_locID, (float)width, (float)height);

        glUniform1ui(renderPassNumber_LocID, PASS_3_2D_EFFECTS_PASS);

        // Set the FBO colour and depth textures to be the texture source for this quad
        {       
            GLuint FSQ_textureUnit = 7;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
            glActiveTexture(FSQ_textureUnit + GL_TEXTURE0);
            GLuint TextureNumber = ::g_pFBO->colourTexture_0_ID;
            glBindTexture(GL_TEXTURE_2D, TextureNumber);

            // uniform sampler2D texture_07;
            GLint FSQ_textureSamplerID = glGetUniformLocation(program, "texture_07");
            glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);
        }

        {
            GLuint FSQ_textureUnit = 8;	    // We picked 8 because it is next to 7.
            glActiveTexture(FSQ_textureUnit + GL_TEXTURE0);
            GLuint TextureNumber = ::g_pFBO->depthTexture_ID;
            glBindTexture(GL_TEXTURE_2D, TextureNumber);

            GLint FSQ_textureSamplerID = glGetUniformLocation(program, "texture_08");
            glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);
        }

        {
            GLuint FSQ_textureUnit = 9;	    // We picked 9 because it is next to 8.
            glActiveTexture(FSQ_textureUnit + GL_TEXTURE0);
            GLuint TextureNumber = ::g_pFBO->vertexNormal_2_ID;
            glBindTexture(GL_TEXTURE_2D, TextureNumber);

            GLint FSQ_textureSamplerID = glGetUniformLocation(program, "texture_09");
            glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);
        }

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

//detail::tmat4x4<T> glm::gtc::matrix_transform::ortho	(	T const & 	left,
//                                                         T const & 	right,
//                                                         T const & 	bottom,
//                                                         T const & 	top,
//                                                         T const & 	zNear,
//                                                         T const & 	zFar )		
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

        matView = glm::ortho(
            -1.f / (float)width / 5.f,   // Left
            -5.f / (float)width / 100.f,  // Right
            0.f,   // Top
            1.0f / (float)height/ 40.f, // Bottom
            30.0f, // zNear  Eye is at 450, quad is at 500, so 50 units away
            70.0f); // zFar

        glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
            1, GL_FALSE, glm::value_ptr(matView));

        glUniform1ui(renderPassNumber_LocID, PASS_4_UI_PASS);

        // "Present" what we've drawn.
        glfwSwapBuffers(pWindow);        // Show what we've drawn

        // Process any events that have happened
        glfwPollEvents();

        // Handle OUR keyboard, mouse stuff
        handleAsyncKeyboard(pWindow, deltaTime);
        handleAsyncMouse(pWindow, deltaTime);

    }
    ::g_pGameEngine->Destroy();

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
        ::g_pDebugSphere->scale = glm::vec3(5.0f);
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

        ::g_pDebugSphere->scale = glm::vec3(0.1f);
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
        const std::string LOW_RES_SPHERE_MODEL = "ISO_Shphere_flat_3div_xyz_n_rgba_uv.ply";
        const std::string HIGH_RES_SPHERE_MODEL = "ISO_Shphere_flat_4div_xyz_n_rgba_uv.ply";

        //float calcApproxDistFromAtten( 
        //      float targetLightLevel, 
        //      float accuracy, 
        //      float infiniteDistance, 
        //      float constAttenuation, 
        //      float linearAttenuation,  
        //      float quadraticAttenuation, 
        //	    unsigned int maxIterations = DEFAULTMAXITERATIONS /*= 50*/ );

                // How far away is 95% brightness?
        float distTo95Percent = ::g_pTheLights->lightHelper.calcApproxDistFromAtten(0.95f,    /* the target light level I want*/
            0.01f,    /*accuracy - how close to 0.25f*/
            10000.0f, /*infinity away*/
            currentLight.atten.x, /*const atten*/
            currentLight.atten.y, /*linear atten*/
            currentLight.atten.z, /*quadratic atten*/
            cLightHelper::DEFAULTMAXITERATIONS);
        // Draw a red sphere at 95%
        ::g_pDebugSphere->scale = glm::vec3(distTo95Percent);
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

        ::g_pDebugSphere->setAllMeshNames(::g_pDebugSphere->scale.x < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);

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
        ::g_pDebugSphere->scale = glm::vec3(distTo50Percent);
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        ::g_pDebugSphere->setAllMeshNames(::g_pDebugSphere->scale.x < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
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
        ::g_pDebugSphere->scale = glm::vec3(distTo25Percent);
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        ::g_pDebugSphere->setAllMeshNames(::g_pDebugSphere->scale.x < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
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
        ::g_pDebugSphere->scale = glm::vec3(distTo5Percent);
        ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        ::g_pDebugSphere->setAllMeshNames(::g_pDebugSphere->scale.x < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(::g_pDebugSphere,
            matModelDS,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);


        ::g_pDebugSphere->setAllMeshNames( oldDebugSphereModel);

    }//if ( ::g_bShowDebugShere )

    return;
}