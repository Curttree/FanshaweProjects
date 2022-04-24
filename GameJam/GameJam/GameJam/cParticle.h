#pragma once
#include <cEntity.h>
class cParticle : public cEntity {
public:
	cParticle(std::string texture, float _lifeSpan, glm::vec3 _position, float transparency, bool followCameraPosition, glm::vec3 _scale= glm::vec3(1.f), glm::vec3 _velocity = glm::vec3(0.f));
	virtual void TimeStep(float deltaTime);
	std::vector<std::string> textures;
	glm::vec3 velocity = glm::vec3(0.f);
private:
	float aliveTimer = 0.f;
	float lifeSpan = 5.f;
	bool followCamPos;
	void OrientToCamera();
	std::string GetCurrentTexture();
};