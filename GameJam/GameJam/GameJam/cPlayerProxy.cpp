#include "cPlayerProxy.h"
#include "globals.h"
#include <iostream>

cPlayerProxy::cPlayerProxy(glm::vec3 position) {
    cMesh* proxy_mesh = new cMesh("can.ply");
    proxy_mesh->scale = glm::vec3(1.f,2.5f,1.f);
    proxy_mesh->positionXYZ = position;
    proxy_mesh->textureNames[0] = "red.bmp";
    proxy_mesh->textureRatios[0] = 1.f;
    proxy_mesh->alphaTransparency = 0.5f;

    mesh = proxy_mesh;
    this->position = proxy_mesh->positionXYZ;
    this->rotation = glm::quat(proxy_mesh->orientationXYZ);
    this->scale = glm::vec3(1.f,2.5f,1.f);
    rotation = proxy_mesh->orientationXYZ;

    walkSpeed = 7.f;
    runSpeed = 15.f;

    InitializePhysics(eShapeType::Cylinder, 50.f, this->scale);
}

void cPlayerProxy::TimeStep(float deltaTime) {
    if (mesh && rigidBody && !rigidBody->IsStatic()) {
        glm::vec3 movement = glm::vec3(0.f);
        if (ApplyPlayerMovement(movement)) {
            rigidBody->SetVelocity(glm::normalize(movement) * (::g_pGameEngine->entityManager.GetPlayer()->isRunning() ? runSpeed : walkSpeed));
            rotation = glm::quat(glm::vec3(0.f, atan2(movement.x, movement.z), 0.f));
        }
        else {
            rigidBody->SetVelocity(glm::vec3(0.f));
        }
        position = rigidBody->GetPosition();
        velocity = movement;
    }
    cEntity::TimeStep(deltaTime);
}

void cPlayerProxy::InitializePhysics(eShapeType shape, float mass, glm::vec3 scale) {

    gdp2022Physics::iShape* cylinderShape = new gdp2022Physics::CylinderShape(scale);
    gdp2022Physics::cRigidBodyDesc cylinderDesc;
    cylinderDesc.linearDamping = 1.00f;
    cylinderDesc.angularDamping = 1.0f;
    cylinderDesc.isStatic = false;
    cylinderDesc.mass = mass;
    cylinderDesc.position = position;
    cylinderDesc.linearVelocity = glm::vec3(0.f);
    cylinderDesc.restitution = 0.8f;
    cylinderDesc.rotation = glm::quat(glm::vec3(0.f));

    rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(cylinderDesc, cylinderShape);

    if (rigidBody != 0 && ::g_pGameEngine->m_PhysicsWorld) {
        ::g_pGameEngine->m_PhysicsWorld->AddBody(rigidBody);
    }
}

void cPlayerProxy::ResetOrientation() {
    this->rotation = glm::quat(glm::vec3(0.f));
}