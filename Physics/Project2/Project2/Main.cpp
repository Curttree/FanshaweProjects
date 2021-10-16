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

#include "cFirework.h"
#include "cWorldSpace.h"
#include "cMathHelper.h"
#include "sFireworkObject.h"

//DEBUG FOR MEMORY LEAK INVESTIGATION
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#pragma region Globals

cVAOManager     gVAOManager;
cShaderManager  gShaderManager;

cModel g_groundModel;

cWorldSpace* worldSpace = cWorldSpace::Instance();
cMathHelper* mathHelper = cMathHelper::Instance();

configManager* _configManager = new configManager();

glm::vec3 cameraEye = _configManager->_cameraStartingPosition;

std::vector<sFireworkObject*> particleObjs;
glm::mat3 axes;

//TODO: Separate into own class?

GLuint program = 0;     // 0 means "no shader program"
float ratio;
int width, height;
glm::mat4 matModel;    // used to be "m"; Sometimes it's called "world"
glm::mat4 p;
glm::mat4 v;
glm::mat4 mvp;    
GLint matModel_Location;
GLint matView_Location;
GLint matProjection_Location;

#pragma endregion

//TODO: Determine if there is a better place for this.
void renderModel(cModel model);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void captureCameraPosition() {
    std::cout << "Camera is currently positioned at x: " << cameraEye.x << " y: " << cameraEye.y << " z: " << cameraEye.z << std::endl;
}

void InitProject1Variables(cFirework* particle)
{
    // because our "sphere" has a radius of 1
    glm::vec3 position(0.0, 1.1f, 0.0);
    glm::vec3 velocity = (worldSpace->axes[0] * mathHelper->getRandom(-2.f, 2.f)) + (worldSpace->axes[1] * 5.f) + (worldSpace->axes[2] * mathHelper->getRandom(-2.f, 2.f));
    velocity = glm::normalize(velocity);
    velocity *= 50.f;
    particle->SetPosition(position);
    particle->SetVelocity(velocity);
}

void InitFirework() {
    //TODO: Separate into own class.
    //Add model to vector.
    sFireworkObject* newObj = new sFireworkObject();
    cModel* firework = new cModel();
    firework->modelName = "assets/pokeball.ply";
    firework->bOverriveVertexColourHACK = true;
    firework->bIsWireframe = false;
    firework->positionXYZ = glm::vec3(0.f, 1.0f, 0.f);
    newObj->model = firework;
    //Add particle to particle list
    cFirework* newParticle = new cFirework(1.0f, glm::vec3(0.f, 1.f, 0.f));
    worldSpace->_world->GetForceRegistry()->Register(newParticle, worldSpace->_gravityGenerator);
    newObj->particle = newParticle;

    particleObjs.push_back(newObj);

    if (worldSpace->_world->AddParticle(newParticle))
    {
        std::cout << "Hurray!" << std::endl;
    }

    InitProject1Variables(newParticle);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float cameraSpeed = 0.3f;

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

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        captureCameraPosition();
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        InitFirework();
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        _CrtDumpMemoryLeaks();
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

void updatePositions(std::vector<sFireworkObject*> particles) {
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

    window = glfwCreateWindow(1200, 640, "INFO-6019 - Project 2 (Curtis Tremblay-1049697)", NULL, NULL);

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

    for (sFireworkObject* p : particleObjs)
    {
        InitProject1Variables(p->particle);
    }

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


        worldSpace->_world->TimeStep(deltaTime);

        // Safety, mostly for first frame
        if (deltaTime == 0.f)
        {
            deltaTime = 0.03f;
        }

        for (unsigned int index = 0; index != particleObjs.size(); index++)
        {
            renderModel(*particleObjs[index]->model);
        }

        renderModel(g_groundModel);
        updatePositions(particleObjs);

        // Scene is drawn

        for (int x = 0; x < particleObjs.size(); x++)
        {
            if (particleObjs[x]->particle->isReadyForStageTwo()) {
                delete particleObjs[x];
                particleObjs[x] = 0;
                particleObjs.erase(particleObjs.begin() + x);
                // Decrement x. Size of vector shrunk, so index has decreased by 1.
                if (x > 0) {
                    x--;
                }
            }
        }

        // "Present" what we've drawn.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shutDown(window);
}

void renderModel(cModel model) {
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
