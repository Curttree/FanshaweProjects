#pragma once
#include <cEntity.h>
class cParticle : public cEntity {
public:
	cParticle(std::string texture, float _lifeSpan, glm::vec3 _position, bool followCameraPosition, glm::vec3 _scale= glm::vec3(1.f));
	virtual void TimeStep(float deltaTime);
private:
	float aliveTimer = 0.f;
	float lifeSpan = 5.f;
	bool followCamPos;
	void OrientToCamera();
};