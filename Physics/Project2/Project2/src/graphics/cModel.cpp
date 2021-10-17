#include "cModel.h"

cModel::cModel()
{
	this->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	this->scale = 1.0f;

	this->bIsWireframe = false;

	this->vertexColourOverrideHACK = glm::vec3(1.0f, 1.0f, 1.0f);	// White
	this->bOverriveVertexColourHACK = false;

    this->bDontLightObject = false;
}

glm::mat4 cModel::buildWorldMatrix() {
    glm::mat4 identity = glm::mat4(1.f);	// Identity Matrix.
	glm::mat4 result = identity;

    // Translate, then rotate, then scale (convention so each is applied properly)
    glm::mat4 matTranslate = glm::translate(identity, positionXYZ);

    glm::mat4 rotateZ = glm::rotate(identity, orientationXYZ.z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotateY = glm::rotate(identity, orientationXYZ.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotateX = glm::rotate(identity, orientationXYZ.x, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 matScale = glm::scale(identity, glm::vec3(scale, scale, scale));

    result *= matTranslate;
    result *= rotateZ;
    result *= rotateY;
    result *= rotateX;
    result *= matScale;
	return result;
}