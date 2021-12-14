#pragma once
#include "iMessage.h"
#include "eState.h"
#include <extern/glm/vec3.hpp> // glm::vec3

class iTank : public iMessage {
public:
	virtual void TimeStep(float deltaTime) = 0;
	virtual bool CheckValidMove(glm::vec3 newPos, glm::vec3 heading) = 0;
	virtual void MoveLeftRight_X(float deltaTime, float amount) = 0;
	virtual void MoveUpDown_Y(float deltaTime, float amount) = 0;
	virtual glm::vec3 GetPosition() = 0;
	virtual TankState GetState() = 0;
	virtual int GetId() = 0;
	virtual glm::vec3 GetHeading() = 0;
	virtual void SnapToGrid(glm::vec3 position) = 0;
	virtual void StopMoving() = 0;
};