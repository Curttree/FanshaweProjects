#include "cInvaderMediator.h"
#include "cEntity.h"
#include <iostream>

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
	}
	return true;
}
bool cInvaderMediator::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	return true;
}
bool cInvaderMediator::SetReciever(iMediator* pTheReciever) {
	return true;
}