#include "cSuperTank.h"
#include "globals.h"
#include <iostream>

cSuperTank::cSuperTank(cMesh* _model) : cAITank(_model) {
	startingHealth *= 1.5f;
	health *= 1.5f;
	damageModifier = 1.5f;
	model->wholeObjectDiffuseRGBA = startingColour;
}


void cSuperTank::TimeStep(float deltaTime) {
	if (activeSuperBullet) {
		activeSuperBullet->TimeStep(deltaTime);
	}
	cAITank::TimeStep(deltaTime);
}

void cSuperTank::FireSomething() {
	int choice = gGetRandBetween(0, 3);
	if (choice == 0) {
		if (!activeBullet && !coolDownActive) {
			activeBullet = new cBullet(GetId(), model->positionXYZ, heading, damageModifier);
			activeBullet->SetReciever(p_Mediator);
		}
	}
	if (choice == 1) {
		if (!activeSuperBullet && !coolDownActive) {
			std::cout << "SUPER!" << std::endl;
			activeSuperBullet = new cSuperBullet(GetId(), model->positionXYZ, heading, damageModifier);
			activeSuperBullet->SetReciever(p_Mediator);
		}
	}
	else {
		state = TankState::CHARGING;
	}
}

bool cSuperTank::RecieveMessage(sMessage theMessage)  {
	cAITank::RecieveMessage(theMessage);
	if (theMessage.command == "CURVE SUPERBULLET") {
		if (activeSuperBullet) {
			activeSuperBullet->Redirect();
		}
	}
	else if (theMessage.command == "DESTROY SUPERBULLET") {
		if (activeSuperBullet) {
			delete activeSuperBullet;
			activeSuperBullet = 0;
			coolDownActive = true;
		}
	}
	return true;
}