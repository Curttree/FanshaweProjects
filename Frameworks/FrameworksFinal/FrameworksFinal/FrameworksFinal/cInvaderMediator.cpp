#include "cInvaderMediator.h"
#include "cEntity.h"
#include <iostream>
#include "globals.h"

// Not technically mediator pattern. Should broadcast the messages to the individuals rather than handling

bool cInvaderMediator::RecieveMessage(sMessage theMessage) {
	if (theMessage.command == "Hit") {
		cEntity* entityOne = (cEntity*)theMessage.vec_voidPData[0];
		cEntity* entityTwo = (cEntity*)theMessage.vec_voidPData[1];
		if (entityTwo->mesh->friendlyName == "Bullet" && entityOne->mesh->friendlyName == "Alien") {
			// Aliens are spawned before the bullet, so they will always be entityOne.
			sMessage destroy;
			destroy.command = "Destroy";
			entityOne->RecieveMessage(destroy);
			entityTwo->RecieveMessage(destroy);
			sMessage speed;
			speed.command = "Speed Up Ships"; 
			speed.vec_voidPData.push_back(theMessage.vec_voidPData[0]);
			::g_pGameState->RecieveMessage(speed);
		}
		if (entityTwo->mesh->friendlyName == "Bullet" && entityOne->mesh->friendlyName == "UFO" ||
			entityOne->mesh->friendlyName == "Bullet" && entityTwo->mesh->friendlyName == "UFO") {
			// Aliens are spawned before the bullet, so they will always be entityOne.
			sMessage destroy;
			destroy.command = "Destroy";
			entityOne->RecieveMessage(destroy);
			entityTwo->RecieveMessage(destroy);
		}
		if (entityTwo->mesh->friendlyName == "Bullet" && entityOne->mesh->friendlyName == "Shield" ||
			entityOne->mesh->friendlyName == "Bullet" && entityTwo->mesh->friendlyName == "Shield") {
			// Aliens are spawned before the bullet, so they will always be entityOne.
			sMessage destroy;
			destroy.command = "Destroy";
			entityOne->RecieveMessage(destroy);
			entityTwo->RecieveMessage(destroy);
		}
		if (entityTwo->mesh->friendlyName == "Shield" && entityOne->mesh->friendlyName == "Alien") {
			// Aliens are spawned before the bullet, so they will always be entityOne.
			sMessage destroy;
			destroy.command = "Destroy";
			entityTwo->RecieveMessage(destroy);
		}
		if (entityTwo->mesh->friendlyName == "Missile" && entityOne->mesh->friendlyName == "Shield" ||
			entityOne->mesh->friendlyName == "Missile" && entityTwo->mesh->friendlyName == "Shield") {
			// Aliens are spawned before the bullet, so they will always be entityOne.
			sMessage destroy;
			destroy.command = "Destroy";
			entityOne->RecieveMessage(destroy);
			entityTwo->RecieveMessage(destroy);
		}
		if (entityTwo->mesh->friendlyName == "Missile" && entityOne->mesh->friendlyName == "Player" ||
			entityOne->mesh->friendlyName == "Missile" && entityTwo->mesh->friendlyName == "Player") {
			// Aliens are spawned before the bullet, so they will always be entityOne.
			sMessage destroy;
			destroy.command = "Destroy";
			entityOne->RecieveMessage(destroy);
			entityTwo->RecieveMessage(destroy);
			sMessage playerdeath;
			playerdeath.command = "Player Died";
			::g_pGameState->RecieveMessage(playerdeath);
		}
	}
	else if (theMessage.command == "Reverse Ships") {
		::g_pGameState->RecieveMessage(theMessage);
	}
	else if (theMessage.command == "Game Over") {
		::g_pGameState->RecieveMessage(theMessage);
	}
	return true;
}
bool cInvaderMediator::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	return true;
}
bool cInvaderMediator::SetReciever(iMediator* pTheReciever) {
	return true;
}