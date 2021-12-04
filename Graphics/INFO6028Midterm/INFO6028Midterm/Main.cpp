#pragma once

#include <iostream>
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp> // glm::vec3
#include <extern/glm/vec4.hpp> // glm::vec4
#include <extern/glm/mat4x4.hpp> // glm::mat4
#include <extern/glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <extern/glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "globals.h"

float SPOTLIGHT_INNER = 30.f;
float SPOTLIGHT_OUTER = 50.f;

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

void EnvironmentModelSetup();

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

void Shutdown(GLFWwindow* pWindow){
    ::g_ShutDown(pWindow);
    glfwDestroyWindow(pWindow);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

int main(void) {
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

    pWindow = glfwCreateWindow(1200, 640, "Curtis Tremblay - Game Development Sandbox", NULL, NULL);

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


    cShaderManager::cShader vertShader;
    cShaderManager::cShader fragShader;
    InitShaders(program, vertShader, fragShader);

    glUseProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    GLint matModel_Location = glGetUniformLocation(program, "matModel");
    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
    GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");

    // LIGHTS 
//    	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//	                // 0 = pointlight
//					// 1 = spot light
//					// 2 = directional light
// 
    // Hallway lights
    ::g_pTheLights->theLights[0].position = glm::vec4(0.f, 24.f, 25.f, 1.f);
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[0].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].atten = glm::vec4(0.001f, 0.036f, 0.0003f, 100.f);
    ::g_pTheLights->theLights[0].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[0].param1.y = 10.0f;
    ::g_pTheLights->theLights[0].param1.z = 20.0f;
    ::g_pTheLights->TurnOnLight(0);

    ::g_pTheLights->theLights[1].position = glm::vec4(0.f, 25.f, 75.f, 1.f);
    ::g_pTheLights->theLights[1].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[1].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[1].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[1].atten = glm::vec4(0.001f, 0.036f, 0.0003f, 100.f);
    ::g_pTheLights->theLights[1].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[1].param1.y = 10.0f;
    ::g_pTheLights->theLights[1].param1.z = 20.0f;
    ::g_pTheLights->TurnOnLight(1);

    ::g_pTheLights->theLights[2].position = glm::vec4(0.f, 24.f, 125.f, 1.f);
    ::g_pTheLights->theLights[2].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[2].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[2].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[2].atten = glm::vec4(0.001f, 0.036f, 0.0003f, 100.f);
    ::g_pTheLights->theLights[2].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[2].param1.y = 10.0f;
    ::g_pTheLights->theLights[2].param1.z = 20.0f;
    ::g_pTheLights->TurnOnLight(2);

    ::g_pTheLights->theLights[3].position = glm::vec4(0.f, 24.f, 175.f, 1.f);
    ::g_pTheLights->theLights[3].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[3].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[3].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[3].atten = glm::vec4(0.001f, 0.036f, 0.0003f, 100.f);
    ::g_pTheLights->theLights[3].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[3].param1.y = 10.0f;
    ::g_pTheLights->theLights[3].param1.z = 20.0f;
    ::g_pTheLights->TurnOnLight(3);

    ::g_pTheLights->theLights[4].position = glm::vec4(10.f, 365.f, 500.f, 1.f);
    ::g_pTheLights->theLights[4].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[4].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[4].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[4].atten = glm::vec4(0.005f, 0.05f, 0.0001f, 200.f);
    ::g_pTheLights->theLights[4].param1.x = 0.0f;    // point light
    ::g_pTheLights->theLights[4].param1.y = 1.0f;
    ::g_pTheLights->theLights[4].param1.z = 100.0f;
    ::g_pTheLights->TurnOnLight(4);

    // HARD SPOT LIGHT
    ::g_pTheLights->theLights[5].position = glm::vec4(0.f, 200.f, 600.f, 1.f);
    ::g_pTheLights->theLights[5].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[5].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[5].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[5].atten = glm::vec4(0.01f, 0.001f, 0.0001f, 50.f);
    ::g_pTheLights->theLights[5].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[5].param1.y = 15;
    ::g_pTheLights->theLights[5].param1.z = 20;
    ::g_pTheLights->TurnOnLight(5);
    
    // SOFT SPOT LIGHTS
    ::g_pTheLights->theLights[6].position = glm::vec4(-100.f, 200.f, 600.f, 1.f);
    ::g_pTheLights->theLights[6].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[6].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[6].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[6].atten = glm::vec4(0.001f, 0.003f, 0.0001f, 100.f);
    ::g_pTheLights->theLights[6].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[6].param1.y = SPOTLIGHT_INNER;
    ::g_pTheLights->theLights[6].param1.z = SPOTLIGHT_OUTER;
    ::g_pTheLights->TurnOnLight(6);

    ::g_pTheLights->theLights[7].position = glm::vec4(0.f, 200.f, 300.f, 1.f);
    ::g_pTheLights->theLights[7].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[7].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[7].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[7].atten = glm::vec4(0.001f, 0.003f, 0.0001f, 100.f);
    ::g_pTheLights->theLights[7].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[7].param1.y = SPOTLIGHT_INNER;
    ::g_pTheLights->theLights[7].param1.z = SPOTLIGHT_OUTER;
    ::g_pTheLights->TurnOffLight(7);

    ::g_pTheLights->theLights[8].position = glm::vec4(-100.f, 200.f, 300.f, 1.f);
    ::g_pTheLights->theLights[8].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[8].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[8].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[8].atten = glm::vec4(0.001f, 0.003f, 0.0001f, 100.f);
    ::g_pTheLights->theLights[8].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[8].param1.y = SPOTLIGHT_INNER;
    ::g_pTheLights->theLights[8].param1.z = SPOTLIGHT_OUTER;
    ::g_pTheLights->TurnOnLight(8);

    ::g_pTheLights->theLights[9].position = glm::vec4(100.f, 200.f, 300.f, 1.f);
    ::g_pTheLights->theLights[9].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[9].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[9].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[9].atten = glm::vec4(0.001f, 0.003f, 0.0001f, 100.f);
    ::g_pTheLights->theLights[9].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[9].param1.y = SPOTLIGHT_INNER;
    ::g_pTheLights->theLights[9].param1.z = SPOTLIGHT_OUTER;
    ::g_pTheLights->TurnOnLight(9);

    ::g_pTheLights->theLights[10].position = glm::vec4(100.f, 200.f, 600.f, 1.f);
    ::g_pTheLights->theLights[10].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[10].specular = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[10].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[10].atten = glm::vec4(0.001f, 0.003f, 0.0001f, 100.f);
    ::g_pTheLights->theLights[10].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[10].param1.y = SPOTLIGHT_INNER;
    ::g_pTheLights->theLights[10].param1.z = SPOTLIGHT_OUTER;
    ::g_pTheLights->TurnOnLight(10);

    // ALARM LIGHT
    ::g_pTheLights->theLights[11].position = glm::vec4(0.f, 200.f, 600.f, 1.f);
    ::g_pTheLights->theLights[11].diffuse = glm::vec4(1.f, 1.f, 0.8f, 1.0f);
    ::g_pTheLights->theLights[11].specular = glm::vec4(1.f, 0.f, 0.f, 1.0f);
    ::g_pTheLights->theLights[11].direction = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    ::g_pTheLights->theLights[11].atten = glm::vec4(0.001f, 0.003f, 0.0001f, 100.f);
    ::g_pTheLights->theLights[11].param1.x = 1.0f;    // spot light
    ::g_pTheLights->theLights[11].param1.y = 1.f;
    ::g_pTheLights->theLights[11].param1.z = 100.f;

    // Get the uniform locations of the light shader values
    ::g_pTheLights->SetUpUniformLocations(program);

    // Set up the debug sphere object
    ::g_pDebugSphere = new cMesh();
    ::g_pDebugSphere->meshName = "Sphere_xyz_n_rgba.ply";
    ::g_pDebugSphere->bIsWireframe = true;
    ::g_pDebugSphere->bUseObjectDebugColour = true;
    ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pDebugSphere->bDontLight = true;
    ::g_pDebugSphere->scale = 1.0f;
    ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
    // Give this a friendly name
    ::g_pDebugSphere->friendlyName = "Debug Sphere";
    // Then I could make a small function that searches for that name

    ::g_pVAOManager->setFilePath("assets/models/");
    std::vector<std::string> vecModelsToLoad;
    vecModelsToLoad.push_back("Sphere_xyz_n_rgba.ply");
    vecModelsToLoad.push_back("ISO_Shphere_flat_3div_xyz_n_rgba.ply");
    vecModelsToLoad.push_back("ISO_Shphere_flat_4div_xyz_n_rgba.ply");
    vecModelsToLoad.push_back("SM_Env_Ceiling_Light_02_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Door_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Floor_04_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Transition_Door_Curved_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Wall_Curved_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Wall_Curved_02_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Wall_Curved_03_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Wall_Curved_04_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Wall_Curved_05_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Cart_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Lockers_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Rocket_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Scales_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_ServerRack_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_StepLadder_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_SwivelChair_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_SwivelChair_04_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Construction_Wall_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Construction_HalfWall_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Env_Floor_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Beaker_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_02_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_03_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_04_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_Lab_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_Lab_02_xyz_n_rgba_uv.ply");


    //Enviornment setup
    EnvironmentModelSetup();


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

    // Slight variation on star generation code from class.
    const unsigned int NUMBER_OF_STARS = 500;
    unsigned int starCount = 0;
    float maxPickDistance = 1'000'000.0f;
    while (starCount < NUMBER_OF_STARS)
    {
        glm::vec3 starLocation = glm::vec3(
            gGetRandBetween<float>(-maxPickDistance/2, maxPickDistance/2),
            gGetRandBetween<float>(-maxPickDistance / 2, maxPickDistance/2),
            gGetRandBetween<float>(maxPickDistance/2, maxPickDistance));
        // Far enough?
        if (glm::distance(glm::vec3(0.0f), starLocation) >= maxPickDistance * 0.8f)
        {
            cMesh* pStar = new cMesh();
            pStar->positionXYZ = starLocation;
            pStar->scale = gGetRandBetween<float>(200.0f, 500.0f);
            pStar->bUseWholeObjectDiffuseColour = true;
            // Pick some star colours
            // 90% white
            // 7.5% Yellow, red, or orange
            // 2.5% blue white
            float starColour = gGetRandBetween<float>(0.0f, 1.0f);
            if (starColour <= 0.025f)
            {   // 2.5% blue white
                pStar->wholeObjectDiffuseRGBA = glm::vec4(gGetRandBetween<float>(0.9f, 1.0f), 1.0f, 0.0f, 1.0f);
            }
            else if (starColour <= 0.075f)
            {   // 7.5% Yellow, red, or orange
                pStar->wholeObjectDiffuseRGBA = glm::vec4(1.0f, gGetRandBetween<float>(0.5f, 1.0f), 0.0f, 1.0f);
            }
            else
            {   // 90% white
                pStar->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            }

            pStar->bDontLight = true;
            //pStar->bIsWireframe = true;
            pStar->meshName = "ISO_Shphere_flat_3div_xyz_n_rgba.ply";
            // Add it
            ::g_vec_pMeshes.push_back(pStar);
            //
            starCount++;
        }
    }//while (starCount < NUMBER_OF_STARS)

    //TODO: Load lights and models.

    const double MAX_DELTA_TIME = 0.1;  // 100 ms
    double previousTime = glfwGetTime();


    while (!glfwWindowShouldClose(pWindow)) {
        float ratio;
        int width, height;
        glm::mat4 matModel;             // used to be "m"; Sometimes it's called "world"
        glm::mat4 matProjection;        // used to be "p";
        glm::mat4 matView;              // used to be "v";

        double currentTime = glfwGetTime();
        double deltaTime = previousTime - currentTime;
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


        // Copy the light information into the shader to draw the scene
        ::g_pTheLights->CopyLightInfoToShader();

        ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[::g_selectedLight].position;
        // Place the "debug sphere" at the same location as the selected light (again)
        // HACK: Debug sphere is 5th item added
//        ::g_vecMeshes[5].positionXYZ = gTheLights.theLights[0].position;

        matProjection = glm::perspective(
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


        if (g_doorsMoving && g_doorsOpened) {
            g_CloseDoor(deltaTime);
        }
        else if (g_doorsMoving && !g_doorsOpened){
            g_OpenDoor(deltaTime);
        }

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
        for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
        {
            // So the code is a little easier...
            cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            //mat4x4_identity(m);

            DrawObject(pCurrentMesh,
                matModel,
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                ::g_pVAOManager);


        }//for (unsigned int index
        // Scene is drawn
        // **********************************************************************   


        //DrawDebugObjects(matModel_Location, matModelInverseTranspose_Location, program, ::g_pVAOManager);


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
        const std::string LOW_RES_SPHERE_MODEL = "ISO_Shphere_flat_3div_xyz_n_rgba.ply";
        const std::string HIGH_RES_SPHERE_MODEL = "ISO_Shphere_flat_4div_xyz_n_rgba.ply";

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

glm::vec4 GetColour(std::string colourName) {
    // Could be separated to its own class/changed to a more relevant structure.
    if (colourName == "white") {
        return glm::vec4(1.f, 1.f, 1.f, 1.0f);
    }
    else if (colourName == "offwhite") {
        return glm::vec4(0.8f, 0.8f, 0.8f, 0.8f);
    }
    else if (colourName == "grey") {
        return glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
    }
    else if (colourName == "black") {
        return glm::vec4(0.f, 0.f, 0.f, 0.f);
    }
    else if (colourName == "green") {
        return glm::vec4(0.f, 1.f, 0.f, 0.f);
    }
    else if (colourName == "red") {
        return glm::vec4(1.f, 0.f, 0.f, 0.f);
    }
    else if (colourName == "brown") {
        return glm::vec4(0.6f, 0.3f, 0.f, 0.f);
    }
    else {
        // No valid selection. Return white.
        return glm::vec4(1.f, 1.f, 1.f, 1.0f);
    }
}

cMesh* GenFloorTile(glm::vec3 position) {
    cMesh* floorTile = new cMesh();
    floorTile->meshName = "SM_Env_Floor_01_xyz_n_rgba_uv.ply";
    floorTile->positionXYZ = position;
    floorTile->bUseWholeObjectDiffuseColour = true;
    floorTile->scale = 10.f;
    floorTile->wholeObjectDiffuseRGBA = GetColour("offwhite");
    return floorTile;
}

void GenWallStack(glm::vec3 position, glm::vec3 orientation, std::vector<unsigned int>& ids, int additionalHeight = 200 ) {
    //TODO: Name walls so can refer back to for part 4.
    for (int y = position.y; y <= position.y+ additionalHeight; y += 50) {
        cMesh* wall = new cMesh();
        wall->meshName = "SM_Env_Construction_Wall_01_xyz_n_rgba_uv.ply";
        wall->positionXYZ = glm::vec3(position.x, y, position.z);
        wall->orientationXYZ = orientation;
        wall->bUseWholeObjectDiffuseColour = true;
        wall->scale = 10.f;
        wall->wholeObjectDiffuseRGBA = GetColour("offwhite");
        ::g_vec_pMeshes.push_back(wall);
        ids.push_back(wall->getUniqueID());
    }
}

void EnvironmentModelSetup() {
    // =========== QUESTION 1 : Setup corridor==============================

    //SECTION 1
    cMesh* curvedWall_sec0_l = new cMesh();
    curvedWall_sec0_l->meshName = "SM_Env_Wall_Curved_01_xyz_n_rgba_uv.ply";
    curvedWall_sec0_l->positionXYZ = glm::vec3(25.f, -25.f, 50.f);
    curvedWall_sec0_l->orientationXYZ = glm::vec3(0.f, -glm::pi<float>()/2, 0.f);
    curvedWall_sec0_l->scale = 10.f;
    curvedWall_sec0_l->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec0_l->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec0_r = new cMesh();
    curvedWall_sec0_r->meshName = "SM_Env_Wall_Curved_02_xyz_n_rgba_uv.ply";
    curvedWall_sec0_r->positionXYZ = glm::vec3(-25.f, -25.f, 0.f);
    curvedWall_sec0_r->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec0_r->scale = 10.f;
    curvedWall_sec0_r->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec0_r->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec0_floor = new cMesh();
    curvedWall_sec0_floor->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec0_floor->positionXYZ = glm::vec3(-25.f, -25.f, 0.f);
    curvedWall_sec0_floor->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec0_floor->scale = 10.f;
    curvedWall_sec0_floor->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec0_floor->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec0_ceiling = new cMesh();
    curvedWall_sec0_ceiling->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec0_ceiling->positionXYZ = glm::vec3(-25.f, 25.f, 0.f);
    curvedWall_sec0_ceiling->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec0_ceiling->scale = 10.f;
    curvedWall_sec0_ceiling->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec0_ceiling->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec0_light = new cMesh();
    curvedWall_sec0_light->meshName = "SM_Env_Ceiling_Light_02_xyz_n_rgba_uv.ply";
    curvedWall_sec0_light->positionXYZ = glm::vec3(0.f, 25.f, 25.f);
    curvedWall_sec0_light->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec0_light->scale = 10.f;
    curvedWall_sec0_light->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec0_light->wholeObjectSpecularRGB = GetColour("white");

    ::g_vec_pMeshes.push_back(curvedWall_sec0_l);
    ::g_vec_pMeshes.push_back(curvedWall_sec0_r);
    ::g_vec_pMeshes.push_back(curvedWall_sec0_floor);
    ::g_vec_pMeshes.push_back(curvedWall_sec0_ceiling);
    ::g_vec_pMeshes.push_back(curvedWall_sec0_light);

    //SECTION 1
    cMesh* curvedWall_sec1_l = new cMesh();
    curvedWall_sec1_l->meshName = "SM_Env_Wall_Curved_01_xyz_n_rgba_uv.ply";
    curvedWall_sec1_l->positionXYZ = glm::vec3(25.f, -25.f, 100.f);
    curvedWall_sec1_l->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2, 0.f);
    curvedWall_sec1_l->scale = 10.f;
    curvedWall_sec1_l->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec1_l->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec1_r = new cMesh();
    curvedWall_sec1_r->meshName = "SM_Env_Wall_Curved_04_xyz_n_rgba_uv.ply";
    curvedWall_sec1_r->positionXYZ = glm::vec3(-25.f, -25.f, 50.f);
    curvedWall_sec1_r->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec1_r->scale = 10.f;
    curvedWall_sec1_r->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec1_r->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec1_floor = new cMesh();
    curvedWall_sec1_floor->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec1_floor->positionXYZ = glm::vec3(-25.f, -25.f, 50.f);
    curvedWall_sec1_floor->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec1_floor->scale = 10.f;
    curvedWall_sec1_floor->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec1_floor->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec1_ceiling = new cMesh();
    curvedWall_sec1_ceiling->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec1_ceiling->positionXYZ = glm::vec3(-25.f, 25.f, 50.f);
    curvedWall_sec1_ceiling->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec1_ceiling->scale = 10.f;
    curvedWall_sec1_ceiling->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec1_ceiling->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec1_light = new cMesh();
    curvedWall_sec1_light->meshName = "SM_Env_Ceiling_Light_02_xyz_n_rgba_uv.ply";
    curvedWall_sec1_light->positionXYZ = glm::vec3(0.f, 25.f, 75.f);
    curvedWall_sec1_light->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec1_light->scale = 10.f;
    curvedWall_sec1_light->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec1_light->wholeObjectDiffuseRGBA = GetColour("white");

    ::g_vec_pMeshes.push_back(curvedWall_sec1_l);
    ::g_vec_pMeshes.push_back(curvedWall_sec1_r);
    ::g_vec_pMeshes.push_back(curvedWall_sec1_floor);
    ::g_vec_pMeshes.push_back(curvedWall_sec1_ceiling);
    ::g_vec_pMeshes.push_back(curvedWall_sec1_light);

    //SECTION 2
    cMesh* curvedWall_sec2_l = new cMesh();
    curvedWall_sec2_l->meshName = "SM_Env_Wall_Curved_01_xyz_n_rgba_uv.ply";
    curvedWall_sec2_l->positionXYZ = glm::vec3(25.f, -25.f, 150.f);
    curvedWall_sec2_l->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2, 0.f);
    curvedWall_sec2_l->scale = 10.f;
    curvedWall_sec2_l->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec2_l->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec2_r = new cMesh();
    curvedWall_sec2_r->meshName = "SM_Env_Wall_Curved_01_xyz_n_rgba_uv.ply";
    curvedWall_sec2_r->positionXYZ = glm::vec3(-25.f, -25.f, 100.f);
    curvedWall_sec2_r->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec2_r->scale = 10.f;
    curvedWall_sec2_r->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec2_r->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec2_floor = new cMesh();
    curvedWall_sec2_floor->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec2_floor->positionXYZ = glm::vec3(-25.f, -25.f, 100.f);
    curvedWall_sec2_floor->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec2_floor->scale = 10.f;
    curvedWall_sec2_floor->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec2_floor->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec2_ceiling = new cMesh();
    curvedWall_sec2_ceiling->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec2_ceiling->positionXYZ = glm::vec3(-25.f, 25.f, 100.f);
    curvedWall_sec2_ceiling->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec2_ceiling->scale = 10.f;
    curvedWall_sec2_ceiling->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec2_ceiling->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec2_light = new cMesh();
    curvedWall_sec2_light->meshName = "SM_Env_Ceiling_Light_02_xyz_n_rgba_uv.ply";
    curvedWall_sec2_light->positionXYZ = glm::vec3(0.f, 25.f, 125.f);
    curvedWall_sec2_light->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec2_light->scale = 10.f;
    curvedWall_sec2_light->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec2_light->wholeObjectSpecularRGB = GetColour("white");

    ::g_vec_pMeshes.push_back(curvedWall_sec2_l);
    ::g_vec_pMeshes.push_back(curvedWall_sec2_r);
    ::g_vec_pMeshes.push_back(curvedWall_sec2_floor);
    ::g_vec_pMeshes.push_back(curvedWall_sec2_ceiling);
    ::g_vec_pMeshes.push_back(curvedWall_sec2_light);

    //SECTION 3
    cMesh* curvedWall_sec3_l = new cMesh();
    curvedWall_sec3_l->meshName = "SM_Env_Wall_Curved_03_xyz_n_rgba_uv.ply";
    curvedWall_sec3_l->positionXYZ = glm::vec3(25.f, -25.f, 200.f);
    curvedWall_sec3_l->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2, 0.f);
    curvedWall_sec3_l->scale = 10.f;
    curvedWall_sec3_l->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec3_l->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec3_r = new cMesh();
    curvedWall_sec3_r->meshName = "SM_Env_Wall_Curved_05_xyz_n_rgba_uv.ply";
    curvedWall_sec3_r->positionXYZ = glm::vec3(-25.f, -25.f, 150.f);
    curvedWall_sec3_r->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec3_r->scale = 10.f;
    curvedWall_sec3_r->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec3_r->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec3_floor = new cMesh();
    curvedWall_sec3_floor->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec3_floor->positionXYZ = glm::vec3(-25.f, -25.f, 150.f);
    curvedWall_sec3_floor->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec3_floor->scale = 10.f;
    curvedWall_sec3_floor->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec3_floor->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec3_ceiling = new cMesh();
    curvedWall_sec3_ceiling->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec3_ceiling->positionXYZ = glm::vec3(-25.f, 25.f, 150.f);
    curvedWall_sec3_ceiling->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec3_ceiling->scale = 10.f;
    curvedWall_sec3_ceiling->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec3_ceiling->wholeObjectSpecularRGB = GetColour("offwhite");

    cMesh* curvedWall_sec3_light = new cMesh();
    curvedWall_sec3_light->meshName = "SM_Env_Ceiling_Light_02_xyz_n_rgba_uv.ply";
    curvedWall_sec3_light->positionXYZ = glm::vec3(0.f, 25.f, 175.f);
    curvedWall_sec3_light->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec3_light->scale = 10.f;
    curvedWall_sec3_light->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec3_light->wholeObjectSpecularRGB = GetColour("white");

    ::g_vec_pMeshes.push_back(curvedWall_sec3_l);
    ::g_vec_pMeshes.push_back(curvedWall_sec3_r);
    ::g_vec_pMeshes.push_back(curvedWall_sec3_floor);
    ::g_vec_pMeshes.push_back(curvedWall_sec3_ceiling);
    ::g_vec_pMeshes.push_back(curvedWall_sec3_light);

    cMesh* transitionDoor = new cMesh();
    transitionDoor->meshName = "SM_Env_Door_01_xyz_n_rgba_uv.ply";
    transitionDoor->positionXYZ = glm::vec3(10.f, -25.f, 225.f);
    transitionDoor->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
    transitionDoor->bUseWholeObjectDiffuseColour = true;
    transitionDoor->scale = 10.f;
    transitionDoor->wholeObjectDiffuseRGBA = GetColour("grey");
    transitionDoor->wholeObjectSpecularRGB = GetColour("grey");

    cMesh* transitionDoorFrame = new cMesh();
    transitionDoorFrame->meshName = "SM_Env_Transition_Door_Curved_01_xyz_n_rgba_uv.ply";
    transitionDoorFrame->positionXYZ = glm::vec3(25.f, -25.f, 225.f);
    transitionDoorFrame->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
    transitionDoorFrame->bUseWholeObjectDiffuseColour = true;
    transitionDoorFrame->scale = 10.f;
    transitionDoorFrame->wholeObjectDiffuseRGBA = GetColour("offwhite");
    transitionDoorFrame->wholeObjectSpecularRGB = GetColour("offwhite");

    ::g_vec_pMeshes.push_back(transitionDoor);
    ::g_vec_pMeshes.push_back(transitionDoorFrame);

    // =========== QUESTION 2 : Setup lab ==============================

    // Floor tiles
    for (int z = 200; z <= 750; z += 50) {
        for (int x = -125; x <= 225; x += 50) {
            ::g_vec_pMeshes.push_back(GenFloorTile(glm::vec3((float)x, -25.f, (float)z)));
        }
    }

    // Ceiling tiles
    for (int z = 200; z <= 750; z += 50) {
        for (int x = -125; x <= 225; x += 50) {
            ::g_vec_pMeshes.push_back(GenFloorTile(glm::vec3((float)x, 225.f, (float)z)));
        }
    }
    std::vector<unsigned int> temp;
    // Front wall (near entrance)
    GenWallStack(glm::vec3(25.f, -25.f, 200.f), glm::vec3(0.f), temp);
    GenWallStack(glm::vec3(125.f, -25.f, 200.f), glm::vec3(0.f), temp);
    GenWallStack(glm::vec3(-75.f, 25.f, 200.f), glm::vec3(0.f), temp, 150);
    GenWallStack(glm::vec3(-175.f, -25.f, 200.f), glm::vec3(0.f), temp);

    // Back wall
    GenWallStack(glm::vec3(-75.f, -25.f, 800.f), glm::vec3(0.f, glm::pi<float>(), 0.f), wallPanel1);
    GenWallStack(glm::vec3(25.f, -25.f, 800.f), glm::vec3(0.f, glm::pi<float>(), 0.f), wallPanel2);
    GenWallStack(glm::vec3(125.f, -25.f, 800.f), glm::vec3(0.f, glm::pi<float>(), 0.f), wallPanel3);
    GenWallStack(glm::vec3(225.f, -25.f, 800.f), glm::vec3(0.f, glm::pi<float>(), 0.f), wallPanel4);

    // Right wall
    GenWallStack(glm::vec3(-175.f, -25.f, 800.f), glm::vec3(0.f, glm::pi<float>()/2, 0.f), temp);
    GenWallStack(glm::vec3(-175.f, -25.f, 700.f), glm::vec3(0.f, glm::pi<float>() / 2, 0.f), temp);
    GenWallStack(glm::vec3(-175.f, -25.f, 600.f), glm::vec3(0.f, glm::pi<float>() / 2, 0.f), temp);
    GenWallStack(glm::vec3(-175.f, -25.f, 500.f), glm::vec3(0.f, glm::pi<float>() / 2, 0.f), temp);
    GenWallStack(glm::vec3(-175.f, -25.f, 400.f), glm::vec3(0.f, glm::pi<float>() / 2, 0.f), temp);
    GenWallStack(glm::vec3(-175.f, -25.f, 300.f), glm::vec3(0.f, glm::pi<float>() / 2, 0.f), temp);

    // Left wall
    GenWallStack(glm::vec3(225.f, -25.f, 700.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f), temp);
    GenWallStack(glm::vec3(225.f, -25.f, 600.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f), temp);
    GenWallStack(glm::vec3(225.f, -25.f, 500.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f), temp);
    GenWallStack(glm::vec3(225.f, -25.f, 400.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f), temp);
    GenWallStack(glm::vec3(225.f, -25.f, 300.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f), temp);
    GenWallStack(glm::vec3(225.f, -25.f, 200.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f), temp);

    cMesh* halfwall = new cMesh();
    halfwall->meshName = "SM_Env_Construction_HalfWall_01_xyz_n_rgba_uv.ply";
    halfwall->positionXYZ = glm::vec3(-75.f, -25.f, 200.f);
    halfwall->bUseWholeObjectDiffuseColour = true;
    halfwall->scale = 10.f;
    halfwall->wholeObjectDiffuseRGBA = GetColour("offwhite");
    ::g_vec_pMeshes.push_back(halfwall);

    cMesh* lab_light1 = new cMesh();
    lab_light1->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light1->positionXYZ = glm::vec3(0.f, 200.f, 300.f);
    lab_light1->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light1->scale = 10.f;
    lab_light1->bUseWholeObjectDiffuseColour = true;
    lab_light1->wholeObjectDiffuseRGBA = GetColour("white");
    lab_light1->wholeObjectSpecularRGB = GetColour("white");

    cMesh* lab_light2 = new cMesh();
    lab_light2->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light2->positionXYZ = glm::vec3(100.f, 200.f, 300.f);
    lab_light2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light2->scale = 10.f;
    lab_light2->bUseWholeObjectDiffuseColour = true;
    lab_light2->wholeObjectDiffuseRGBA = GetColour("white");
    lab_light2->wholeObjectSpecularRGB = GetColour("white");

    cMesh* lab_light3 = new cMesh();
    lab_light3->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light3->positionXYZ = glm::vec3(0.f, 200.f, 600.f);
    lab_light3->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light3->scale = 10.f;
    lab_light3->bUseWholeObjectDiffuseColour = true;
    lab_light3->wholeObjectDiffuseRGBA = GetColour("white");
    lab_light3->wholeObjectSpecularRGB = GetColour("white");

    cMesh* lab_light4 = new cMesh();
    lab_light4->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light4->positionXYZ = glm::vec3(100.f, 200.f, 600.f);
    lab_light4->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light4->scale = 10.f;
    lab_light4->bUseWholeObjectDiffuseColour = true;
    lab_light4->wholeObjectDiffuseRGBA = GetColour("white");
    lab_light4->wholeObjectSpecularRGB = GetColour("white");

    cMesh* lab_light5 = new cMesh();
    lab_light5->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light5->positionXYZ = glm::vec3(-100.f, 200.f, 300.f);
    lab_light5->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light5->scale = 10.f;
    lab_light5->bUseWholeObjectDiffuseColour = true;
    lab_light5->wholeObjectDiffuseRGBA = GetColour("white");
    lab_light5->wholeObjectSpecularRGB = GetColour("white");

    cMesh* lab_light6 = new cMesh();
    lab_light6->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light6->positionXYZ = glm::vec3(-100.f, 200.f, 600.f);
    lab_light6->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light6->scale = 10.f;
    lab_light6->bUseWholeObjectDiffuseColour = true;
    lab_light6->wholeObjectDiffuseRGBA = GetColour("white");
    lab_light6->wholeObjectSpecularRGB = GetColour("white");

    ::g_vec_pMeshes.push_back(lab_light1);
    ::g_vec_pMeshes.push_back(lab_light2);
    ::g_vec_pMeshes.push_back(lab_light3);
    ::g_vec_pMeshes.push_back(lab_light4);
    ::g_vec_pMeshes.push_back(lab_light5);
    ::g_vec_pMeshes.push_back(lab_light6);


    // =========== QUESTION 3 : Populate lab ==============================

    cMesh* lab_desk1 = new cMesh();
    lab_desk1->meshName = "SM_Prop_Desk_Lab_01_xyz_n_rgba_uv.ply";
    lab_desk1->positionXYZ = glm::vec3(-120.f, -25.f, 600.f);
    lab_desk1->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_desk1->scale = 10.f;
    lab_desk1->bUseWholeObjectDiffuseColour = true;
    lab_desk1->wholeObjectSpecularRGB = GetColour("brown");
    lab_desk1->wholeObjectDiffuseRGBA = GetColour("brown");

    cMesh* lab_desk2 = new cMesh();
    lab_desk2->meshName = "SM_Prop_Desk_Lab_02_xyz_n_rgba_uv.ply";
    lab_desk2->positionXYZ = glm::vec3(100.f, -25.f, 400.f);
    lab_desk2->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2, 0.f);
    lab_desk2->scale = 10.f;
    lab_desk2->bUseWholeObjectDiffuseColour = true;
    lab_desk2->wholeObjectSpecularRGB = GetColour("brown");
    lab_desk2->wholeObjectDiffuseRGBA = GetColour("brown");

    cMesh* desk1 = new cMesh();
    desk1->meshName = "SM_Prop_Desk_01_xyz_n_rgba_uv.ply";
    desk1->positionXYZ = glm::vec3(100.f, -25.f, 600.f);
    desk1->orientationXYZ = glm::vec3(0.f,  glm::pi<float>() / 4, 0.f);
    desk1->scale = 10.f;
    desk1->bUseWholeObjectDiffuseColour = true;
    desk1->wholeObjectSpecularRGB = GetColour("brown");
    desk1->wholeObjectDiffuseRGBA = GetColour("brown");

    cMesh* desk2 = new cMesh();
    desk2->meshName = "SM_Prop_Desk_04_xyz_n_rgba_uv.ply";
    desk2->positionXYZ = glm::vec3(-100.f, -25.f, 400.f);
    desk2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 4, 0.f);
    desk2->scale = 10.f;
    desk2->bUseWholeObjectDiffuseColour = true;
    desk2->wholeObjectSpecularRGB = GetColour("brown");
    desk2->wholeObjectDiffuseRGBA = GetColour("brown");

    cMesh* beaker1 = new cMesh();
    beaker1->meshName = "SM_Prop_Beaker_01_xyz_n_rgba_uv.ply";
    beaker1->positionXYZ = glm::vec3(100.f, -10.f, 600.f);
    beaker1->orientationXYZ = glm::vec3(0.f, -3 * glm::pi<float>() / 4, 0.f);
    beaker1->scale = 10.f;
    beaker1->bUseWholeObjectDiffuseColour = true;
    beaker1->wholeObjectDiffuseRGBA = glm::vec4(1.f, 0.f, 0.f, 0.01f);

    cMesh* beaker2 = new cMesh();
    beaker2->meshName = "SM_Prop_Beaker_01_xyz_n_rgba_uv.ply";
    beaker2->positionXYZ = glm::vec3(95.f, -10.f, 605.f);
    beaker2->orientationXYZ = glm::vec3(0.f, -3 * glm::pi<float>() / 4, 0.f);
    beaker2->scale = 10.f;
    beaker2->bUseWholeObjectDiffuseColour = true;
    beaker2->wholeObjectDiffuseRGBA = glm::vec4(1.f, 0.f, 0.f, 0.01f);

    cMesh* beaker3 = new cMesh();
    beaker3->meshName = "SM_Prop_Beaker_01_xyz_n_rgba_uv.ply";
    beaker3->positionXYZ = glm::vec3(105.f, -10.f, 595.f);
    beaker3->orientationXYZ = glm::vec3(0.f, -3 * glm::pi<float>() / 4, 0.f);
    beaker3->scale = 10.f;
    beaker3->bUseWholeObjectDiffuseColour = true;
    beaker3->wholeObjectDiffuseRGBA = glm::vec4(1.f, 0.f, 0.f, 0.01f);

    cMesh* beaker4 = new cMesh();
    beaker4->meshName = "SM_Prop_Beaker_01_xyz_n_rgba_uv.ply";
    beaker4->positionXYZ = glm::vec3(110.f, -10.f, 590.f);
    beaker4->orientationXYZ = glm::vec3(0.f, -3 * glm::pi<float>() / 4, 0.f);
    beaker4->scale = 10.f;
    beaker4->bUseWholeObjectDiffuseColour = true;
    beaker4->wholeObjectDiffuseRGBA = glm::vec4(1.f, 0.f, 0.f, 0.01f);

    cMesh* cart = new cMesh();
    cart->meshName = "SM_Prop_Cart_01_xyz_n_rgba_uv.ply";
    cart->positionXYZ = glm::vec3(-82.5f, -25.f, 382.5f);
    cart->orientationXYZ = glm::vec3(0.f, 3 * glm::pi<float>() / 4, 0.f);
    cart->scale = 10.f;
    cart->bUseWholeObjectDiffuseColour = true;
    cart->wholeObjectSpecularRGB = GetColour("grey");
    cart->wholeObjectDiffuseRGBA = GetColour("grey");

    cMesh* stepladder = new cMesh();
    stepladder->meshName = "SM_Prop_StepLadder_01_xyz_n_rgba_uv.ply";
    stepladder->positionXYZ = glm::vec3(-60.f, -25.f, 560.f);
    stepladder->orientationXYZ = glm::vec3(0.f, -3 *glm::pi<float>() / 4, 0.f);
    stepladder->scale = 10.f;
    stepladder->bUseWholeObjectDiffuseColour = true;
    stepladder->wholeObjectSpecularRGB = GetColour("grey");
    stepladder->wholeObjectDiffuseRGBA = GetColour("grey");

    cMesh* scales = new cMesh();
    scales->meshName = "SM_Prop_Scales_01_xyz_n_rgba_uv.ply";
    scales->positionXYZ = glm::vec3(-95.f, -10.f, 395.f);
    scales->orientationXYZ = glm::vec3(0.f, -3* glm::pi<float>() / 4, 0.f);
    scales->scale = 10.f;
    scales->bUseWholeObjectDiffuseColour = true;
    scales->wholeObjectSpecularRGB = GetColour("grey");
    scales->wholeObjectDiffuseRGBA = GetColour("grey");

    cMesh* stepladder2 = new cMesh();
    stepladder2->meshName = "SM_Prop_StepLadder_01_xyz_n_rgba_uv.ply";
    stepladder2->positionXYZ = glm::vec3(40.f, -25.f, 560.f);
    stepladder2->orientationXYZ = glm::vec3(0.f, 3 * glm::pi<float>() / 4, 0.f);
    stepladder2->scale = 10.f;
    stepladder2->bUseWholeObjectDiffuseColour = true;
    stepladder2->wholeObjectSpecularRGB = GetColour("grey");
    stepladder2->wholeObjectDiffuseRGBA = GetColour("grey");

    cMesh* rocket = new cMesh();
    rocket->meshName = "SM_Prop_Rocket_01_xyz_n_rgba_uv.ply";
    rocket->positionXYZ = glm::vec3(0.f, -25.f, 590.f);
    rocket->orientationXYZ = glm::vec3(0.f, 0.f, 0.f);
    rocket->scale = 10.f;
    rocket->bUseWholeObjectDiffuseColour = true;
    rocket->wholeObjectSpecularRGB = GetColour("red");
    rocket->wholeObjectDiffuseRGBA = GetColour("red");

    cMesh* locker1 = new cMesh();
    locker1->meshName = "SM_Prop_Lockers_01_xyz_n_rgba_uv.ply";
    locker1->positionXYZ = glm::vec3(215.f, -30.f, 700.f);
    locker1->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2, 0.f);
    locker1->scale = 10.f;
    locker1->bUseWholeObjectDiffuseColour = true;
    locker1->wholeObjectSpecularRGB = GetColour("green");
    locker1->wholeObjectDiffuseRGBA = GetColour("green");

    cMesh* locker2 = new cMesh();
    locker2->meshName = "SM_Prop_Lockers_01_xyz_n_rgba_uv.ply";
    locker2->positionXYZ = glm::vec3(215.f, -30.f, 680.f);
    locker2->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2, 0.f);
    locker2->scale = 10.f;
    locker2->bUseWholeObjectDiffuseColour = true;
    locker2->wholeObjectSpecularRGB = GetColour("green");
    locker2->wholeObjectDiffuseRGBA = GetColour("green");

    cMesh* chair = new cMesh();
    chair->meshName = "SM_Prop_SwivelChair_01_xyz_n_rgba_uv.ply";
    chair->positionXYZ = glm::vec3(-105.f, -25.f, 600.f);
    chair->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2, 0.f);
    chair->scale = 10.f;
    chair->bUseWholeObjectDiffuseColour = true;
    chair->wholeObjectSpecularRGB = GetColour("black");
    chair->wholeObjectDiffuseRGBA = GetColour("black");

    cMesh* server = new cMesh();
    server->meshName = "SM_Prop_ServerRack_01_xyz_n_rgba_uv.ply";
    server->positionXYZ = glm::vec3(-165.f, -25.f, 500.f);
    server->orientationXYZ = glm::vec3(0.f, -glm::pi<float>() / 2, 0.f);
    server->scale = 10.f;
    server->bUseWholeObjectDiffuseColour = true;
    server->wholeObjectSpecularRGB = GetColour("grey");
    server->wholeObjectDiffuseRGBA = GetColour("grey");

    cMesh* chair2 = new cMesh();
    chair2->meshName = "SM_Prop_SwivelChair_04_xyz_n_rgba_uv.ply";
    chair2->positionXYZ = glm::vec3(85.f, -25.f, 400.f);
    chair2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    chair2->scale = 10.f;
    chair2->bUseWholeObjectDiffuseColour = true;
    chair2->wholeObjectSpecularRGB = GetColour("black");
    chair2->wholeObjectDiffuseRGBA = GetColour("black");

    ::g_vec_pMeshes.push_back(lab_desk1);
    ::g_vec_pMeshes.push_back(lab_desk2);
    ::g_vec_pMeshes.push_back(desk1);
    ::g_vec_pMeshes.push_back(desk2);
    ::g_vec_pMeshes.push_back(beaker1);
    ::g_vec_pMeshes.push_back(beaker2);
    ::g_vec_pMeshes.push_back(beaker3);
    ::g_vec_pMeshes.push_back(beaker4);

    ::g_vec_pMeshes.push_back(cart);
    ::g_vec_pMeshes.push_back(stepladder);
    ::g_vec_pMeshes.push_back(scales);
    ::g_vec_pMeshes.push_back(stepladder2);
    ::g_vec_pMeshes.push_back(rocket);
    ::g_vec_pMeshes.push_back(locker1);
    ::g_vec_pMeshes.push_back(locker2);
    ::g_vec_pMeshes.push_back(chair);
    ::g_vec_pMeshes.push_back(server);
    ::g_vec_pMeshes.push_back(chair2);

    return;
}