#include "cPlayerTank.h"
#include <iostream>
#include <extern/glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective


cPlayerTank::cPlayerTank(cMesh* _model) {
	model = _model;
}

bool cPlayerTank::RecieveMessage(sMessage theMessage) {

	return true;
}

bool cPlayerTank::RecieveMessage(sMessage theMessage, sMessage& theResponse) {

	return true;
}

bool cPlayerTank::SetReciever(iMediator* pTheReciever) {
	this->p_Mediator = pTheReciever;
	return true;
}

glm::vec3 cPlayerTank::GetPosition() {
	return model->positionXYZ;
}

TankState cPlayerTank::GetState() {
	return state;
}

void cPlayerTank::TimeStep(float deltaTime) {
	//PSEUDOCODE:
	//CHECK NEXT POSITION BASED ON SPEED * DELTATIME
	//CALL CHECK COLLISIONS WHICH FIRES MESSAGE TO THE MEDIATOR WITH NEW POSITION (MEDIATOR CAN EITHER TRACK MAZE OR CALL ANOTHER CLASS THAT DOES)
	//MEDIATOR RETURNS YES OR NO. ON NO DO NOTHING, ON YES CALL APPROPRIATE MOVEMENT AND ROTATE IF NEEDED.
}	
bool cPlayerTank::CheckValidMove(glm::vec3 newPos, glm::vec3 heading) {
	if (p_Mediator) {
		sMessage outgoingMessage;
		sMessage responseMessage;
		outgoingMessage.command = "VALIDATE MOVE";
		outgoingMessage.vec_v4Data.push_back(glm::vec4(newPos.x, newPos.y, newPos.z, 1.f));
		outgoingMessage.vec_v4Data.push_back(glm::vec4(heading.x, heading.y, heading.z, 1.f));
		p_Mediator->RecieveMessage(outgoingMessage, responseMessage);
		if (responseMessage.command == "MOVE") {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		// Flying blind. Better not move until someone is directing traffic.
		return false;
	}
}
void cPlayerTank::MoveLeftRight_X(float deltaTime, float amount) {
	state = TankState::MOVING;
	glm::vec3 potential = glm::vec3(model->positionXYZ.x + (deltaTime * amount * speed), model->positionXYZ.y, model->positionXYZ.z);
	if (amount > 0) {
		heading = glm::vec3(1.f, 0.f, 0.f);
		model->orientationXYZ = glm::vec3(-glm::pi<float>() / 2, 0.f, -glm::pi<float>() / 2);
	}
	else {
		heading = glm::vec3(-1.f, 0.f, 0.f);
		model->orientationXYZ = glm::vec3(-glm::pi<float>() / 2, 0.f, glm::pi<float>() / 2);
	}

	if (CheckValidMove(potential, heading)) {
		model->positionXYZ = potential;
		state = TankState::MOVING;
	}
	else {
		//Invalid move. Snap to grid.
		SnapToGrid(potential);
	}
}
void cPlayerTank::MoveUpDown_Y(float deltaTime, float amount) {
	glm::vec3 potential = glm::vec3(model->positionXYZ.x, model->positionXYZ.y + (deltaTime * amount * speed), model->positionXYZ.z);
	if (amount > 0) {
		heading = glm::vec3(0.f, 1.f, 0.f);
		model->orientationXYZ = glm::vec3(-glm::pi<float>() / 2, 0.f, 0.f);
	}
	else {
		heading = glm::vec3(0.f, -1.f, 0.f);
		model->orientationXYZ = glm::vec3(-glm::pi<float>() / 2, 0.f, glm::pi<float>());
	}
	if (CheckValidMove(potential, heading)) {
		model->positionXYZ = potential;
		state = TankState::MOVING;
	}
	else {
		//Invalid move. Snap to grid.
		SnapToGrid(potential);
	}
}

void cPlayerTank::SnapToGrid(glm::vec3 position) {
	if (heading.x != 0.f) {
		model->positionXYZ.x = (float)((int)position.x);
		if (heading.x < 0.f && (float)((int)position.x) == model->positionXYZ.x) {
			model->positionXYZ.x++;
		}
	}
	if (heading.y != 0.f) {
		model->positionXYZ.y = (float)((int)position.y);
		if (heading.y < 0 && (float)((int)position.y) == model->positionXYZ.y) {
			model->positionXYZ.y++;
		}
	}
}

void cPlayerTank::StopMoving() {
	//Moving left
	if (heading.x > 0 && model->positionXYZ.x > (float)(int)model->positionXYZ.x) {
		//move to next square.
		model->positionXYZ.x = (float)(int)model->positionXYZ.x++;
	}
	//Moving right
	if (heading.x < 0 && model->positionXYZ.x > (float)(int)model->positionXYZ.x) {
		//move to next square.
		model->positionXYZ.x = (float)(int)model->positionXYZ.x;
	}
	//Moving up
	if (heading.y > 0 && model->positionXYZ.y > (float)(int)model->positionXYZ.y) {
		//move to next square.
		model->positionXYZ.y = (float)(int)model->positionXYZ.y++;
	}
	//Moving down
	if (heading.y < 0 && model->positionXYZ.y >(float)(int)model->positionXYZ.y) {
		//move to next square.
		model->positionXYZ.y = (float)(int)model->positionXYZ.y;
	}
	state = TankState::WAITING;
}