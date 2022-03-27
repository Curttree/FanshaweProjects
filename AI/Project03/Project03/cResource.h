#pragma once
#include "cMesh.h"
#include "cGatherer.h"

class cResource {
public:
	cResource(glm::vec3 _position);
	virtual ~cResource();
	void Collect(cGatherer* _gatherer);
	void Deliver();
	void Update(float deltaTime);
	bool isActive = true;
	glm::vec3 GetPosition();
	cGatherer* collectedBy;
private:
	cMesh* mesh;
	glm::vec3 position;
};