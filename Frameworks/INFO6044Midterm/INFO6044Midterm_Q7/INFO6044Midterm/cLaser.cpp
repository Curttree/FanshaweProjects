#include "cLaser.h"
#include "globals.h"
#include <iostream>

cLaser::cLaser(int _ownerID, glm::vec3 _position, glm::vec4 _colour, float damageModifier) {
	ownerID = _ownerID;
	model = new cMesh();
	model->meshName = "Sphere_xyz_n_rgba.ply";
	model->positionXYZ = _position;
	model->scale = 0.25f;
	model->bUseWholeObjectDiffuseColour = true;
	model->wholeObjectDiffuseRGBA = _colour;
	::g_vec_pMeshes.push_back(model);
	damage *= damageModifier;
}

cLaser::~cLaser() {
	if (model) {
		int renderIndex = -1;
		for (int x = 0; x < ::g_vec_pMeshes.size(); x++) {
			if (model->getUniqueID() == ::g_vec_pMeshes[x]->getUniqueID()) {
				renderIndex = x;
			}
		}
		if (renderIndex >= 0) {
			::g_vec_pMeshes.erase(::g_vec_pMeshes.begin() + renderIndex);
		}
		delete model;
		model = NULL;
	}
}

void cLaser::TimeStep(float deltaTime) {
	// Check to see if the laser has outlived its usefulness.
	aliveTime += deltaTime;
	if (aliveTime >= lifeSpan) {
		sMessage expiryMessage;
		expiryMessage.command = "LASER EXPIRY";
		expiryMessage.vec_iData.push_back(model->getUniqueID());
		expiryMessage.vec_iData.push_back(ownerID);
		p_Mediator->RecieveMessage(expiryMessage);
	}
	else if (!CheckIfHit()) {
		//We hit something. Mediator already knows and should clean this up.
		std::cout << "Unhandled collision. Laser is still active." << std::endl;
	}
}

int cLaser::GetId() { return model->getUniqueID(); }

bool cLaser::RecieveMessage(sMessage theMessage) {

	return true;
}

bool cLaser::RecieveMessage(sMessage theMessage, sMessage& theResponse) {

	return true;
}

bool cLaser::SetReciever(iMediator* pTheReciever) {
	this->p_Mediator = pTheReciever;
	return true;
}

bool cLaser::CheckIfHit() {
	if (p_Mediator) {
		bool result = true;
		sMessage outgoingMessage;
		sMessage responseMessage;
		outgoingMessage.command = "VALIDATE BULLET HIT";
		outgoingMessage.vec_v4Data.push_back(glm::vec4(model->positionXYZ.x, model->positionXYZ.y, model->positionXYZ.z, 1.f));
		outgoingMessage.vec_iData.push_back(ownerID);
		outgoingMessage.vec_iData.push_back(GetId());
		outgoingMessage.vec_fData.push_back(damage);
		outgoingMessage.vec_sData.push_back("LASER");
		result &= p_Mediator->RecieveMessage(outgoingMessage, responseMessage);
		return result;
	}
	else {
		// Flying blind. Better not move until someone is directing traffic.
		return false;
	}

}