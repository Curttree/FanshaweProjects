#include "cAITank.h"
#include "globals.h"
#include <iostream>
#include <extern/glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective

cAITank::cAITank(cMesh* _model) {
	model = _model;
	startingColour = model->wholeObjectDiffuseRGBA;
}

void cAITank::TimeStep(float deltaTime) {
	if (state == TankState::DEAD) {
		return;
	}
	if (activeBullet) {
		activeBullet->TimeStep(deltaTime);
	}
	for (cLaser* laser : activeLasers) {
		if (laser != 0) {
			laser->TimeStep(deltaTime);
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
	if (state == TankState::CHARGING) {
		currentLaserChargeTime += deltaTime;
		if (currentLaserChargeTime >= maxLaserChargeTime) {
			currentLaserChargeTime = 0.f;
			state = TankState::WAITING;
			FireLaser();
		}
		return;
	}
	if (state == TankState::BLOCKED) {
		waitTime += deltaTime;
		if (waitTime >= maxWaitTime) {
			state = TankState::WAITING;
			waitTime = 0.f;
		}
	}
	if (state == TankState::WAITING) {
		direction = gGetRandBetween(0, 4);
	}
	switch (direction) {
	case(0):
		MoveUpDown_Y(deltaTime, 1.f);
		break;
	case(1):
		MoveUpDown_Y(deltaTime, -1.f);
		break;
	case(2):
		MoveLeftRight_X(deltaTime, 1.f);
		break;
	case(3):
		MoveLeftRight_X(deltaTime, -1.f);
		break;
	default:
		break;
	}
	CheckLineOfSight();
}

bool cAITank::CheckValidMove(glm::vec3 newPos, glm::vec3 heading) {
	//TODO: Check with mediator if will be colliding with other tank.
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

bool cAITank::CheckLineOfSight() {
	if (p_Mediator) {
		sMessage outgoingMessage;
		sMessage responseMessage;
		outgoingMessage.command = "CHECK SIGHT";
		outgoingMessage.vec_iData.push_back(GetId());
		p_Mediator->RecieveMessage(outgoingMessage, responseMessage);
		if (responseMessage.command == "SPOTTED") {
			for (int x = 0; x < responseMessage.vec_iData.size(); x++) {
				if (responseMessage.vec_iData[x] != model->getUniqueID()) {
					// There is someone else here! Fire!!
					FireSomething();
					return true;
				}
			}
		}
		// Just us.
		return false;
	}
	else {
		// Flying blind. Better not fire until someone is directing traffic.
		return false;
	}
}

void cAITank::MoveLeftRight_X(float deltaTime, float amount) {
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
		state = TankState::BLOCKED;
	}
}

void cAITank::MoveUpDown_Y(float deltaTime, float amount) {
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
		state = TankState::BLOCKED;
	}
}

glm::vec3 cAITank::GetPosition() { return model->positionXYZ; }

TankState cAITank::GetState() { return state; }

int cAITank::GetId() { return model->getUniqueID(); }

glm::vec3 cAITank::GetHeading() {
	return heading;
}

void cAITank::SnapToGrid(glm::vec3 position) {
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
void cAITank::StopMoving() {
	return;
}

bool cAITank::RecieveMessage(sMessage theMessage) {
	if (theMessage.command == "DESTROY BULLET") {
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
				std::cout << "AI TANK HAS BEEN ELIMINATED." << std::endl;
				state = TankState::DEAD;
			}
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
	return true;
}

bool cAITank::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	return true; 
}

bool cAITank::SetReciever(iMediator* pTheReciever) {
	this->p_Mediator = pTheReciever;
	return true;
}

void cAITank::FireSomething() {
	int choice = gGetRandBetween(0, 2);
	if (choice == 0) {
		if (!activeBullet && !coolDownActive) {
			activeBullet = new cBullet(GetId(), model->positionXYZ, heading);
			activeBullet->SetReciever(p_Mediator);
		}
	}
	else {
		state = TankState::CHARGING;
	}
}

void cAITank::FireLaser() {
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
				laserPos = glm::vec3(model->positionXYZ.x, model->positionXYZ.y + (float)y, model->positionXYZ.z);
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