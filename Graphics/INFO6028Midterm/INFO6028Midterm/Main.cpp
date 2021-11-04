#pragma once

#include <iostream>
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp> // glm::vec3
#include <extern/glm/vec4.hpp> // glm::vec4
#include <extern/glm/mat4x4.hpp> // glm::mat4
#include <extern/glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <extern/glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "globals.h"

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

    ::g_pTheLights->theLights[0].position = glm::vec4(-5000.0f, 10000.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //... and so on...
//    	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//	                // 0 = pointlight
//					// 1 = spot light
//					// 2 = directional light
//    ::g_pTheLights->theLights[0].param1.x = 1.0f;    // Spot light
    ::g_pTheLights->theLights[0].param1.x = 0.0f;    // point light
    // Direction RELATIVE TO THE LIGHT
    // (-1 in y is straight down)
    ::g_pTheLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].param1.y = 15.0f;   // Inner
    ::g_pTheLights->theLights[0].param1.z = 30.0f;   // Outer

    ::g_pTheLights->theLights[0].atten.y = 0.000001f;
    ::g_pTheLights->theLights[0].atten.z = 0.00000001f;

    //gTheLights.theLights[0].param2.x = 1.0f;
 //   ::g_pTheLights->TurnOnLight(0);  // Or this!

    ::g_pTheLights->theLights[1].position = glm::vec4(0.0f, 25.f, 25.f, 1.0f);
    ::g_pTheLights->theLights[1].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLights->theLights[1].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[1].param1.x = 1.0f;    // point light
    ::g_pTheLights->theLights[1].param1.y = 15.0f;
    ::g_pTheLights->theLights[1].param1.z = 30.0f;
    ::g_pTheLights->TurnOnLight(1);  // Or this!

    ::g_pTheLights->theLights[2].position = glm::vec4(0.0f, 25.f, 25.f, 1.0f);
    ::g_pTheLights->theLights[2].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLights->theLights[2].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[2].param1.x = 0.0f;    // point light
    ::g_pTheLights->theLights[2].param1.y = 100.0f;
    ::g_pTheLights->theLights[2].param1.z = 100.0f;
    ::g_pTheLights->theLights[2].atten = glm::vec4(100.f, 100.f, 100.f, 1.f);
    ::g_pTheLights->TurnOnLight(2);  // Or this!

        // Sunlight: https://encycolorpedia.com/fdfbd3#:~:text=The%20hexadecimal%20color%20code%20%23fdfbd3,%25%20saturation%20and%2091%25%20lightness.
        //The hexadecimal color code #fdfbd3 is a very light shade of yellow. In the RGB color model #fdfbd3 is comprised of 99.22% red, 98.43% green and 82.75% blue. 
    ::g_pTheLights->theLights[0].param1.x = 2.0f;    // directional
    ::g_pTheLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(0.9922f, 0.9843f, 0.8275f, 1.0f);
    ::g_pTheLights->TurnOnLight(0);  // Or this!


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
    vecModelsToLoad.push_back("SM_Env_Floor_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Beaker_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_02_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_03_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_04_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_Lab_01_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("SM_Prop_Desk_Lab_02_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("Mercurio_xyz_n_rgba_x100_Bigger_perturbed_surface.ply");


    //Enviornment setup
    EnvironmentModelSetup();

    cMesh* pPlanetMercury = new cMesh();
    pPlanetMercury->meshName = "Mercurio_xyz_n_rgba_x100_Bigger_perturbed_surface.ply";
    pPlanetMercury->positionXYZ = glm::vec3(100'000.0f, -250'000.0f, 100'000.0f);
    //pPlanetMercury->bIsWireframe = true;
    //pPlanetMercury->bDontLight = true;
    pPlanetMercury->bUseWholeObjectDiffuseColour = true;
    // https://encycolorpedia.com/743027: General Motors Sierra Rust / #743027
    // 45.49% red, 18.82% green and 15.29% blue
    pPlanetMercury->wholeObjectDiffuseRGBA = glm::vec4(0.4549f, 0.1882f, 0.1529f, 1.0f);


    //    ::g_vec_pMeshes.push_back(pTheGround);
    ::g_vec_pMeshes.push_back(pPlanetMercury);

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

        ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
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


        DrawDebugObjects(matModel_Location, matModelInverseTranspose_Location, program, ::g_pVAOManager);


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

void GenWallStack(glm::vec3 position, glm::vec3 orientation, int additionalHeight = 200) {
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
    curvedWall_sec0_l->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec0_r = new cMesh();
    curvedWall_sec0_r->meshName = "SM_Env_Wall_Curved_02_xyz_n_rgba_uv.ply";
    curvedWall_sec0_r->positionXYZ = glm::vec3(-25.f, -25.f, 0.f);
    curvedWall_sec0_r->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec0_r->scale = 10.f;
    curvedWall_sec0_r->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec0_r->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec0_floor = new cMesh();
    curvedWall_sec0_floor->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec0_floor->positionXYZ = glm::vec3(-25.f, -25.f, 0.f);
    curvedWall_sec0_floor->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec0_floor->scale = 10.f;
    curvedWall_sec0_floor->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec0_floor->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec0_ceiling = new cMesh();
    curvedWall_sec0_ceiling->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec0_ceiling->positionXYZ = glm::vec3(-25.f, 25.f, 0.f);
    curvedWall_sec0_ceiling->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec0_ceiling->scale = 10.f;
    curvedWall_sec0_ceiling->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec0_ceiling->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec0_light = new cMesh();
    curvedWall_sec0_light->meshName = "SM_Env_Ceiling_Light_02_xyz_n_rgba_uv.ply";
    curvedWall_sec0_light->positionXYZ = glm::vec3(0.f, 25.f, 25.f);
    curvedWall_sec0_light->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec0_light->scale = 10.f;
    curvedWall_sec0_light->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec0_light->wholeObjectDiffuseRGBA = GetColour("white");

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
    curvedWall_sec2_l->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec2_r = new cMesh();
    curvedWall_sec2_r->meshName = "SM_Env_Wall_Curved_01_xyz_n_rgba_uv.ply";
    curvedWall_sec2_r->positionXYZ = glm::vec3(-25.f, -25.f, 100.f);
    curvedWall_sec2_r->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec2_r->scale = 10.f;
    curvedWall_sec2_r->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec2_r->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec2_floor = new cMesh();
    curvedWall_sec2_floor->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec2_floor->positionXYZ = glm::vec3(-25.f, -25.f, 100.f);
    curvedWall_sec2_floor->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec2_floor->scale = 10.f;
    curvedWall_sec2_floor->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec2_floor->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec2_ceiling = new cMesh();
    curvedWall_sec2_ceiling->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec2_ceiling->positionXYZ = glm::vec3(-25.f, 25.f, 100.f);
    curvedWall_sec2_ceiling->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec2_ceiling->scale = 10.f;
    curvedWall_sec2_ceiling->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec2_ceiling->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec2_light = new cMesh();
    curvedWall_sec2_light->meshName = "SM_Env_Ceiling_Light_02_xyz_n_rgba_uv.ply";
    curvedWall_sec2_light->positionXYZ = glm::vec3(0.f, 25.f, 125.f);
    curvedWall_sec2_light->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec2_light->scale = 10.f;
    curvedWall_sec2_light->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec2_light->wholeObjectDiffuseRGBA = GetColour("white");

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
    curvedWall_sec3_l->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec3_r = new cMesh();
    curvedWall_sec3_r->meshName = "SM_Env_Wall_Curved_05_xyz_n_rgba_uv.ply";
    curvedWall_sec3_r->positionXYZ = glm::vec3(-25.f, -25.f, 150.f);
    curvedWall_sec3_r->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec3_r->scale = 10.f;
    curvedWall_sec3_r->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec3_r->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec3_floor = new cMesh();
    curvedWall_sec3_floor->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec3_floor->positionXYZ = glm::vec3(-25.f, -25.f, 150.f);
    curvedWall_sec3_floor->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec3_floor->scale = 10.f;
    curvedWall_sec3_floor->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec3_floor->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec3_ceiling = new cMesh();
    curvedWall_sec3_ceiling->meshName = "SM_Env_Floor_04_xyz_n_rgba_uv.ply";
    curvedWall_sec3_ceiling->positionXYZ = glm::vec3(-25.f, 25.f, 150.f);
    curvedWall_sec3_ceiling->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec3_ceiling->scale = 10.f;
    curvedWall_sec3_ceiling->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec3_ceiling->wholeObjectDiffuseRGBA = GetColour("offwhite");

    cMesh* curvedWall_sec3_light = new cMesh();
    curvedWall_sec3_light->meshName = "SM_Env_Ceiling_Light_02_xyz_n_rgba_uv.ply";
    curvedWall_sec3_light->positionXYZ = glm::vec3(0.f, 25.f, 175.f);
    curvedWall_sec3_light->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    curvedWall_sec3_light->scale = 10.f;
    curvedWall_sec3_light->bUseWholeObjectDiffuseColour = true;
    curvedWall_sec3_light->wholeObjectDiffuseRGBA = GetColour("white");

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
    transitionDoor->bDontLight = true;

    cMesh* transitionDoorFrame = new cMesh();
    transitionDoorFrame->meshName = "SM_Env_Transition_Door_Curved_01_xyz_n_rgba_uv.ply";
    transitionDoorFrame->positionXYZ = glm::vec3(25.f, -25.f, 225.f);
    transitionDoorFrame->orientationXYZ = glm::vec3(0.f, glm::pi<float>(), 0.f);
    transitionDoorFrame->bUseWholeObjectDiffuseColour = true;
    transitionDoorFrame->scale = 10.f;
    transitionDoorFrame->wholeObjectDiffuseRGBA = GetColour("offwhite");
    transitionDoorFrame->bDontLight = true;

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
    
    // Front wall (near entrance)
    GenWallStack(glm::vec3(25.f, -25.f, 200.f), glm::vec3(0.f));
    GenWallStack(glm::vec3(125.f, -25.f, 200.f), glm::vec3(0.f));
    GenWallStack(glm::vec3(-75.f, 25.f, 200.f), glm::vec3(0.f), 150);
    GenWallStack(glm::vec3(-175.f, -25.f, 200.f), glm::vec3(0.f));

    // Back wall
    GenWallStack(glm::vec3(-75.f, -25.f, 800.f), glm::vec3(0.f, glm::pi<float>(), 0.f));
    GenWallStack(glm::vec3(25.f, -25.f, 800.f), glm::vec3(0.f, glm::pi<float>(), 0.f));
    GenWallStack(glm::vec3(125.f, -25.f, 800.f), glm::vec3(0.f, glm::pi<float>(), 0.f));
    GenWallStack(glm::vec3(225.f, -25.f, 800.f), glm::vec3(0.f, glm::pi<float>(), 0.f));

    // Right wall
    GenWallStack(glm::vec3(-175.f, -25.f, 800.f), glm::vec3(0.f, glm::pi<float>()/2, 0.f));
    GenWallStack(glm::vec3(-175.f, -25.f, 700.f), glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
    GenWallStack(glm::vec3(-175.f, -25.f, 600.f), glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
    GenWallStack(glm::vec3(-175.f, -25.f, 500.f), glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
    GenWallStack(glm::vec3(-175.f, -25.f, 400.f), glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
    GenWallStack(glm::vec3(-175.f, -25.f, 300.f), glm::vec3(0.f, glm::pi<float>() / 2, 0.f));

    // Left wall
    GenWallStack(glm::vec3(225.f, -25.f, 700.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));
    GenWallStack(glm::vec3(225.f, -25.f, 600.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));
    GenWallStack(glm::vec3(225.f, -25.f, 500.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));
    GenWallStack(glm::vec3(225.f, -25.f, 400.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));
    GenWallStack(glm::vec3(225.f, -25.f, 300.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));
    GenWallStack(glm::vec3(225.f, -25.f, 200.f), glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));

    cMesh* lab_light1 = new cMesh();
    lab_light1->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light1->positionXYZ = glm::vec3(0.f, 200.f, 300.f);
    lab_light1->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light1->scale = 10.f;
    lab_light1->bUseWholeObjectDiffuseColour = true;
    lab_light1->wholeObjectDiffuseRGBA = GetColour("red");

    cMesh* lab_light2 = new cMesh();
    lab_light2->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light2->positionXYZ = glm::vec3(100.f, 200.f, 300.f);
    lab_light2->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light2->scale = 10.f;
    lab_light2->bUseWholeObjectDiffuseColour = true;
    lab_light2->wholeObjectDiffuseRGBA = GetColour("red");

    cMesh* lab_light3 = new cMesh();
    lab_light3->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light3->positionXYZ = glm::vec3(0.f, 200.f, 600.f);
    lab_light3->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light3->scale = 10.f;
    lab_light3->bUseWholeObjectDiffuseColour = true;
    lab_light3->wholeObjectDiffuseRGBA = GetColour("red");

    cMesh* lab_light4 = new cMesh();
    lab_light4->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light4->positionXYZ = glm::vec3(100.f, 200.f, 600.f);
    lab_light4->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light4->scale = 10.f;
    lab_light4->bUseWholeObjectDiffuseColour = true;
    lab_light4->wholeObjectDiffuseRGBA = GetColour("red");

    cMesh* lab_light5 = new cMesh();
    lab_light5->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light5->positionXYZ = glm::vec3(-100.f, 200.f, 300.f);
    lab_light5->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light5->scale = 10.f;
    lab_light5->bUseWholeObjectDiffuseColour = true;
    lab_light5->wholeObjectDiffuseRGBA = GetColour("red");

    cMesh* lab_light6 = new cMesh();
    lab_light6->meshName = "SM_Env_Ceiling_Light_01_xyz_n_rgba_uv.ply";
    lab_light6->positionXYZ = glm::vec3(-100.f, 200.f, 600.f);
    lab_light6->orientationXYZ = glm::vec3(0.f, glm::pi<float>() / 2, 0.f);
    lab_light6->scale = 10.f;
    lab_light6->bUseWholeObjectDiffuseColour = true;
    lab_light6->wholeObjectDiffuseRGBA = GetColour("red");


    ::g_vec_pMeshes.push_back(lab_light1);
    ::g_vec_pMeshes.push_back(lab_light2);
    ::g_vec_pMeshes.push_back(lab_light3);
    ::g_vec_pMeshes.push_back(lab_light4);
    ::g_vec_pMeshes.push_back(lab_light5);
    ::g_vec_pMeshes.push_back(lab_light6);

    return;
}