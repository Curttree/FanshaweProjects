#include "cPlayerTank.h"
#include "globals.h"
#include <iostream>
#include <extern/glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective


cPlayerTank::cPlayerTank(cMesh* _model) {
	model = _model;
	startingColour = model->wholeObjectDiffuseRGBA;
}

bool cPlayerTank::RecieveMessage(sMessage theMessage) {

	if (theMessage.command == "FIRE BULLET") {
		if (state != TankState::DEAD && !coolDownActive) {
			if (!activeBullet) {
				activeBullet = new cBullet(GetId(), model->positionXYZ, heading);
				activeBullet->SetReciever(p_Mediator);
			}
		}
	}
	else if (theMessage.command == "FIRE LASER") {
		if (state != TankState::DEAD) {
			state = TankState::CHARGING;
		}
	}
	else if (theMessage.command == "LASER EXPIRY") {
		for (int x = 0; x < activeLasers.size(); x++) {
			if (activeLasers[x]->GetId() == theMessage.vec_iData[0]) {
				laserCleanup.push_back(activeLasers[x]);
				activeLasers.erase(activeLasers.begin() + x);
				break;
			}
		}
	}
	else if (theMessage.command == "DESTROY BULLET") {
		if (activeBullet) {
			delete activeBullet;
			activeBullet = 0;
			coolDownActive = true;
		}
	}
	else if (theMessage.command == "TAKE DAMAGE") {
		if (state != TankState::DEAD) {
			health -= theMessage.vec_fData[0];
			float ratio = health / startingHealth;
			model->wholeObjectDiffuseRGBA = glm::vec4(startingColour.r * ratio, startingColour.g * ratio, startingColour.b * ratio, startingColour.a * ratio);
			if (health <= 0.f) {
				std::cout << "PLAYER TANK HAS BEEN ELIMINATED." << std::endl;
				state = TankState::DEAD;
			}
		}
	}
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

int cPlayerTank::GetId() { return model->getUniqueID(); }

glm::vec3 cPlayerTank::GetHeading() {
	return heading;
}

void cPlayerTank::TimeStep(float deltaTime) {
	if (activeBullet) {
		activeBullet->TimeStep(deltaTime);
	}
	for (cLaser* laser : activeLasers) {
		if (laser != 0) {
			laser->TimeStep(deltaTime);
		}
	}
	if (state == TankState::CHARGING) {
		currentLaserChargeTime += deltaTime;
		if (currentLaserChargeTime >= maxLaserChargeTime) {
			currentLaserChargeTime = 0.f;
			state = TankState::WAITING;
			FireLaser();
		}
	}
	if (laserCleanup.size() > 0) {
		//slowly remove inactive lasers.
		delete laserCleanup[0];
		laserCleanup[0] = 0;
		laserCleanup.erase(laserCleanup.begin());
	}
	if (coolDownActive) {
		currentCoolDown += deltaTime;
		if (currentCoolDown >= coolDown) {
			coolDownActive = false;
			currentCoolDown = 0.f;
		}
	}
}	

void cPlayerTank::FireLaser() {
	sMessage outgoingMessage;
	sMessage responseMessage;
	outgoingMessage.command = "DISTANCE TO WALL";
	outgoingMessage.vec_v4Data.push_back(glm::vec4(model->positionXYZ.x, model->positionXYZ.y, model->positionXYZ.z, 1.f));
	outgoingMessage.vec_v4Data.push_back(glm::vec4(heading.x, heading.y, heading.z, 1.f));
	p_Mediator->RecieveMessage(outgoingMessage, responseMessage);
	if (responseMessage.command == "FIRE AWAY") {
		int dist = responseMessage.vec_iData[0];
		glm::vec3 laserPos;
		if (heading.x > 0.f) {
			// Fire Left
			for (int x = 1; x <= dist; x++) {
				laserPos = glm::vec3(model->positionXYZ.x + (float)x, model->positionXYZ.y, model->positionXYZ.z);
				cLaser* newLaser = new cLaser(GetId(), laserPos, startingColour);
				newLaser->SetReciever(p_Mediator);
				activeLasers.push_back(newLaser);
			}
		}
		else if (heading.x < 0.f) {
			// Fire Right
			for (int x = 1; x <= dist; x++) {
				laserPos = glm::vec3(model->positionXYZ.x - (float)x, model->positionXYZ.y, model->positionXYZ.z);
				cLaser* newLaser = new cLaser(GetId(), laserPos, startingColour);
				newLaser->SetReciever(p_Mediator);
				activeLasers.push_back(newLaser);
			}
		}
		else if (heading.y > 0.f) {
			// Fire Up
			for (int y = 1; y <= dist; y++) {
				laserPos = glm::vec3(model->positionXYZ.x, model->positionXYZ.y+(float)y, model->positionXYZ.z);
				cLaser* newLaser = new cLaser(GetId(), laserPos, startingColour);
				newLaser->SetReciever(p_Mediator);
				activeLasers.push_back(newLaser);
			}
		}
		else if (heading.y < 0.f) {
			// Fire Down
			for (int y = 1; y <= dist; y++) {
				laserPos = glm::vec3(model->positionXYZ.x, model->positionXYZ.y - (float)y, model->positionXYZ.z);
				cLaser* newLaser = new cLaser(GetId(), laserPos, startingColour);
				newLaser->SetReciever(p_Mediator);
				activeLasers.push_back(newLaser);
			}
		}
	}
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
			outgoingMessage.command = "VALIDATE TANK COLLISION";
			outgoingMessage.vec_iData.push_back(model->getUniqueID());
			outgoingMessage.vec_v4Data.push_back(glm::vec4(newPos.x, newPos.y, newPos.z, 1.f));
			outgoingMessage.vec_v4Data.push_back(glm::vec4(heading.x, heading.y, heading.z, 1.f));
			p_Mediator->RecieveMessage(outgoingMessage, responseMessage);
			if (responseMessage.command == "CLEAR") {
				return true;
			}
			else {
				return false;
			}
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
	if (state == TankState::DEAD || state == TankState::CHARGING) {
		return;
	}
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
	if (state == TankState::DEAD || state == TankState::CHARGING) {
		return;
	}
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
	if (state == TankState::DEAD || state == TankState::CHARGING) {
		return;
	}
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