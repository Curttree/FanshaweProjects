#include <extern_includes.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>      // C++ file I-O library (look like any other stream)

#include "src/graphics/cVAOManager.h"
#include "src/graphics/cModel.h"
#include "src/graphics/cShaderManager.h"
#include "src/graphics/configManager.h"

#include <physics/cParticle.h>
#include <physics/cParticleWorld.h>
#include <physics/particle_force_generators.h>

#include "cFireworkBuilder.h"
#include "cFirework.h"
#include "cWorldSpace.h"
#include "cMathHelper.h"
#include "cFireworkObject.h"

#pragma region Globals

cVAOManager     gVAOManager;
cShaderManager  gShaderManager;

cModel g_groundModel;

cWorldSpace* worldSpace = cWorldSpace::Instance();
cMathHelper* mathHelper = cMathHelper::Instance();
cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();

configManager* _configManager = new configManager();

glm::vec3 cameraEye = _configManager->_cameraStartingPosition;

std::vector<cFireworkObject*> particleObjs;

GLuint program = 0;     // 0 means "no shader program"
float ratio;
int width, height;  
GLint matModel_Location;
GLint matView_Location;
GLint matProjection_Location;

#pragma endregion

void renderModel(cModel model);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void captureCameraPosition() {
    std::cout << "Camera is currently positioned at x: " << cameraEye.x << " y: " << cameraEye.y << " z: " << cameraEye.z << std::endl;
}

void InitFirework(int type) {
    glm::vec3 position = (worldSpace->axes[0] * mathHelper->getRandom(-5.f, 5.f)) + (worldSpace->axes[1] * 1.1f) + (worldSpace->axes[2] * mathHelper->getRandom(-5.f, 5.f));
    cFireworkObject* newObj = fireworkBuilder->buildFirework(type,position);

    particleObjs.push_back(newObj);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float cameraSpeed = 10.f;

    switch (key) {
    case GLFW_KEY_A:
    case GLFW_KEY_LEFT:
        cameraEye.x -= cameraSpeed;
        break;
    case GLFW_KEY_D:
    case GLFW_KEY_RIGHT:
        cameraEye.x += cameraSpeed;
        break;
    case GLFW_KEY_W:
    case GLFW_KEY_UP:
        cameraEye.z += cameraSpeed;
        break;
    case GLFW_KEY_S:
    case GLFW_KEY_DOWN:
        cameraEye.z -= cameraSpeed;
        break;
    case GLFW_KEY_E:
        cameraEye.y += cameraSpeed;
        break;
    case GLFW_KEY_Q:
        cameraEye.y -= cameraSpeed;
        break;
    default:
        break;
    }

    if ((key == GLFW_KEY_1 || key == GLFW_KEY_KP_1) && action == GLFW_PRESS)
    {
        InitFirework(1);
    }
    if ((key == GLFW_KEY_2 || key == GLFW_KEY_KP_2) && action == GLFW_PRESS)
    {
        InitFirework(2);
    }
    if ((key == GLFW_KEY_3 || key == GLFW_KEY_KP_3) && action == GLFW_PRESS)
    {
        InitFirework(3);
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        captureCameraPosition();
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraEye.x += xoffset;
    cameraEye.y += yoffset;
}

void shutDown(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();

    if (_configManager) {
        delete _configManager;
        _configManager = nullptr;
    }

    exit(EXIT_SUCCESS);
}

void updatePositions(std::vector<cFireworkObject*> particles) {
    glm::vec3 position;
    for (size_t x = 0; x < particles.size(); x++) {
        particles[x]->particle->GetPosition(position);
        particles[x]->model->positionXYZ = position;
    }
}

void initGround() {
    g_groundModel.modelName = "assets/ground.ply";
    g_groundModel.scale = 1.f;
    g_groundModel.positionXYZ = glm::vec3(0.f, 0.f, 0.f);
    g_groundModel.bOverriveVertexColourHACK = true;
    g_groundModel.bIsWireframe = false;
}

int main(void)
{
    GLFWwindow* window;
    GLint mvp_location = -1;        // Because glGetAttribLocation() returns -1 on error
    float previousTime = static_cast<float>(glfwGetTime());

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1200, 640, "INFO-6044 - Project 1 (Curtis Tremblay-1049697)", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have (still) been omitted for brevity

    cShaderManager::cShader vertShader;
    vertShader.fileName = "assets/shaders/vertShader_01.glsl";

    cShaderManager::cShader fragShader;
    fragShader.fileName = "assets/shaders/fragShader_01.glsl";

    if (gShaderManager.createProgramFromFile("Shader#1", vertShader, fragShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        program = gShaderManager.getIDFromFriendlyName("Shader#1");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << gShaderManager.getLastError() << std::endl;
    }

    mvp_location = glGetUniformLocation(program, "MVP");

    // Get "uniform locations" (aka the registers these are in)
    matModel_Location = glGetUniformLocation(program, "matModel");
    matView_Location = glGetUniformLocation(program, "matView");
    matProjection_Location = glGetUniformLocation(program, "matProjection");

    _configManager->loadModelsIntoVAO(program, gVAOManager);

    initGround();
    float timeElapsed = 0;

    while (!glfwWindowShouldClose(window))
    {
        //        mat4x4 m, p, mvp;
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        // Turn on the depth buffer
        glEnable(GL_DEPTH);         // Turns on the depth buffer
        glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // *******************************************************
        // Screen is cleared and we are ready to draw the scene...
        // *******************************************************

        updatePositions(particleObjs);
        worldSpace->_world->TimeStep(deltaTime);

        // Safety, mostly for first frame
        if (deltaTime == 0.f)
        {
            deltaTime = 0.03f;
        }

        for (int x = 0; x < particleObjs.size(); x++)
        {
            if (particleObjs[x]->fuse->isReadyForNextStage()) {
                std::vector<cFireworkObject*> newFireworks = particleObjs[x]->triggerNextStage();
                if (newFireworks.size() > 0) {
                    particleObjs.insert(particleObjs.end(), newFireworks.begin(), newFireworks.end());
                }
                worldSpace->_world->RemoveParticle(particleObjs[x]->particle);
                delete particleObjs[x];
                particleObjs[x] = 0;
                particleObjs.erase(particleObjs.begin() + x);
                // Decrement x. Size of vector shrunk, so index has decreased by 1.
                x--;
            }
        }

        for (unsigned int index = 0; index != particleObjs.size(); index++)
        {
            renderModel(*particleObjs[index]->model);
        }

        renderModel(g_groundModel);
        // Scene is drawn

        // "Present" what we've drawn.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shutDown(window);
}

void renderModel(cModel model) {
    glm::mat4 matModel;    // used to be "m"; Sometimes it's called "world"
    glm::mat4 p;
    glm::mat4 v;
    glm::mat4 mvp;

    matModel = model.buildWorldMatrix();

    p = glm::perspective(0.6f,
        ratio,
        0.1f,
        1000.0f);

    v = glm::mat4(1.0f);

    glm::vec3 cameraTarget = _configManager->_cameraStartingFocus;
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    v = glm::lookAt(cameraEye,     // "eye"
        cameraTarget,  // "at"
        upVector);

    glUseProgram(program);

    glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));
    glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(v));
    glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(p));

    GLint bUseVertexColour_Location = glGetUniformLocation(program, "bUseVertexColour");
    GLint vertexColourOverride_Location = glGetUniformLocation(program, "vertexColourOverride");
    GLint bDontLightObject_Location = glGetUniformLocation(program, "bDontLightObject");

    glUniform3f(vertexColourOverride_Location, 0.0f, 1.0f, 1.0f);
    glUniform1f(bUseVertexColour_Location, (float)GL_TRUE);

    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (model.bOverriveVertexColourHACK)
    {
        // Override the colour...
        glUniform1f(bUseVertexColour_Location, (float)GL_TRUE);
        glUniform3f(vertexColourOverride_Location,
            model.vertexColourOverrideHACK.r,
            model.vertexColourOverrideHACK.g,
            model.vertexColourOverrideHACK.b);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bUseVertexColour_Location, (float)GL_FALSE);
    }

    // See if mesh is wanting to ignore lighting.
    if (model.bDontLightObject)
    {
        // Override the colour...
        glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
    }

    // Wireframe
    if (model.bIsWireframe)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // Wireframe
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      // Solid-shading
    }

    sModelDrawInfo modelInfo;

    if (gVAOManager.FindDrawInfoByModelName(model.modelName, modelInfo))
    {
        glBindVertexArray(modelInfo.VAO_ID);

        glDrawElements(GL_TRIANGLES,
            modelInfo.numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0);

        glBindVertexArray(0);
    }
}