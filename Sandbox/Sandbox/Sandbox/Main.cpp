#pragma once

#include <iostream>
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp> // glm::vec3
#include <extern/glm/vec4.hpp> // glm::vec4
#include <extern/glm/mat4x4.hpp> // glm::mat4
#include <extern/glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <extern/glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "globals.h"
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

    ::g_pVAOManager->setFilePath("assets/models/");

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


        //TODO: Draw objects     


        //DrawDebugObjects(matModel_Location, matModelInverseTranspose_Location, program, ::g_pVAOManager);


        // "Present" what we've drawn.
        glfwSwapBuffers(pWindow);        // Show what we've drawn

        // Process any events that have happened
        glfwPollEvents();

        // Handle OUR keyboard, mouse stuff

    }

    Shutdown(pWindow);
}
