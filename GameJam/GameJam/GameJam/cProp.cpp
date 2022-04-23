#include "cProp.h"
#include "globals.h"
#include <RigidBody.h>

cProp::cProp(std::string name, std::string texture, glm::vec3 position, eShapeType physicsShape, glm::vec3 scale, glm::vec3 physicsScale, float mass, glm::vec3 orientation, glm::vec3 velocity) {
    cMesh* prop_mesh = new cMesh(name);
    prop_mesh->scale = scale;
    prop_mesh->positionXYZ = position;
	prop_mesh->orientationXYZ = orientation;
    prop_mesh->textureNames[0] = texture;
    prop_mesh->textureRatios[0] = 1.f;
    mesh = prop_mesh;
    this->position = prop_mesh->positionXYZ;
	this->rotation = glm::quat(prop_mesh->orientationXYZ);
    this->scale = prop_mesh->scale;
    rotation = prop_mesh->orientationXYZ;

	InitializePhysics(physicsShape, mass, physicsScale, orientation, velocity);
}

void cProp::TimeStep(float deltaTime) {
	if (mesh && rigidBody && !rigidBody->IsStatic()) {
		rotation = rigidBody->GetOrientation();
		position = rigidBody->GetPosition() + (positionOffset * rotation);
	}
	cEntity::TimeStep(deltaTime);
	if (dying) {
		deathTimer += deltaTime;
		if (deathTimer >= deathMaxTime) {
			::g_pGameEngine->m_PhysicsWorld->RemoveBody(rigidBody);
			::g_pGameEngine->entityManager.DeleteEntity(this);
		}
	}
}

void cProp::InitializePhysics(eShapeType shape, float mass, glm::vec3 scale, glm::vec3 orientation, glm::vec3 velocity) {
	switch (shape) {
	case eShapeType::Plane: {
		gdp2022Physics::iShape* planeShape = new gdp2022Physics::PlaneShape(0,glm::vec3(0.f,1.f,0.f));
		gdp2022Physics::cRigidBodyDesc planeDesc;
		planeDesc.linearDamping = 0.f;
		planeDesc.isStatic = true;
		planeDesc.mass = 0.f;
		planeDesc.rotation = glm::quat(orientation);
		planeDesc.position = glm::vec3(0.f, -3.f, 0.f);
		planeDesc.linearVelocity = glm::vec3(0.f);

		rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(planeDesc, planeShape);
		break;
	}
	case eShapeType::Box: {
		gdp2022Physics::iShape* boxShape = new gdp2022Physics::BoxShape(glm::vec3(scale));
		gdp2022Physics::cRigidBodyDesc boxDesc;
		boxDesc.linearDamping = 0.33f;
		boxDesc.angularDamping = 0.33f;
		boxDesc.isStatic = false;
		boxDesc.mass = mass;
		boxDesc.position = position;
		boxDesc.linearVelocity = velocity;
		boxDesc.restitution = 0.4f;
		boxDesc.rotation = glm::quat(orientation);

		rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(boxDesc, boxShape);
		break;
	}
	case eShapeType::Sphere: {
		gdp2022Physics::iShape* sphereShape = new gdp2022Physics::SphereShape(scale.x);
		gdp2022Physics::cRigidBodyDesc sphereDesc;
		sphereDesc.linearDamping = 0.2;
		sphereDesc.angularDamping = 0.2f;
		sphereDesc.isStatic = false;
		sphereDesc.mass = mass;
		sphereDesc.position = position;
		sphereDesc.linearVelocity = velocity;
		sphereDesc.restitution = 0.4f;

		rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(sphereDesc, sphereShape);
		break;
	}	
	case eShapeType::Cylinder: {
		gdp2022Physics::iShape* cylinderShape = new gdp2022Physics::CylinderShape(scale);
		gdp2022Physics::cRigidBodyDesc cylinderDesc;
		cylinderDesc.linearDamping = 0.33f;
		cylinderDesc.angularDamping = 0.33f;
		cylinderDesc.isStatic = false;
		cylinderDesc.mass = mass;
		cylinderDesc.position = position;
		cylinderDesc.linearVelocity = velocity;
		cylinderDesc.restitution = 0.4f;
		cylinderDesc.rotation = glm::quat(orientation);

		rigidBody = ::g_pGameEngine->m_PhysicsFactory->CreateRigidBody(cylinderDesc, cylinderShape);
		break;
	}
	case (eShapeType::None):
	default: {
		break;
	}
	}

	// Add the Rigid Body to the physics world
	if (rigidBody != 0 && ::g_pGameEngine->m_PhysicsWorld) {
		::g_pGameEngine->m_PhysicsWorld->AddBody(rigidBody);
	}
}

void cProp::Destroy(float inTime) {
	deathMaxTime = inTime;
	dying = true;
}