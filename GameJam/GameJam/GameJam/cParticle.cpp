#include "cParticle.h"
#include "globals.h"

cParticle::cParticle(std::string texture, float _lifeSpan, glm::vec3 _position, bool followCameraPosition, glm::vec3 _scale) : lifeSpan(_lifeSpan), followCamPos(followCameraPosition) {
	cMesh* prop_mesh = new cMesh();
	prop_mesh->setAllMeshNames("Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply");
	prop_mesh->scale = _scale;
	prop_mesh->positionXYZ = _position;
	prop_mesh->textureNames[0] = texture;
	prop_mesh->orientationXYZ = glm::vec3(0.f, 0.f, glm::pi<float>());
	prop_mesh->textureRatios[0] = 1.f;
	mesh = prop_mesh;
	this->position = prop_mesh->positionXYZ;
	this->scale = prop_mesh->scale;
	this->rotation = prop_mesh->orientationXYZ;
}

void cParticle::TimeStep(float deltaTime) {
	OrientToCamera();
	if (followCamPos) {
		this->position = ::g_pFlyCamera->getEye() + g_pFlyCamera->getAtDirection() * (::g_pFlyCamera->nearPlane + 0.1f);
		//position = glm::vec3(0.0f, 0.0f, 490.0f);
		mesh->positionXYZ = this->position;
	}
	if (lifeSpan < 0.f) {
		//Don't bother incrementing, it lives forever.
		return;
	}
	aliveTimer += deltaTime;
	if (aliveTimer > lifeSpan) {
		::g_pGameEngine->entityManager.DeleteParticle(this);
	}
}

void cParticle::OrientToCamera() {
	//glm::vec3 result;
	//glm::vec3 camDirection = ::g_pFlyCamera->getAtDirection();
	//glm::mat4 test = glm::lookAt(mesh->positionXYZ,
	//		-::g_pFlyCamera->getAtDirection(),
	//		::g_pFlyCamera->getUp());
	//result = glm::eulerAngles( glm::quat_cast(test));
	//result.x = mesh->orientationXYZ.z;
	//result.y = atan2(camDirection.x, camDirection.z);
	//result.z = mesh->orientationXYZ.z;

	glm::vec3 v1 = glm::normalize(glm::vec3(0.f, 0.f, -1.f));
	glm::vec3 v2 = -glm::normalize(::g_pFlyCamera->getAtDirection());
	glm::vec3 a = glm::cross(v1, v2);
	glm::quat rotate;
	rotate.x = a.x;
	rotate.y = a.y;
	rotate.z = a.z;
	rotate.w = sqrt((glm::length(v1) * glm::length(v1)) * (glm::length(v2) * glm::length(v2)) + glm::dot(v1, v2));

	this->rotation = rotate;
	mesh->orientationXYZ = glm::eulerAngles(rotate);
}