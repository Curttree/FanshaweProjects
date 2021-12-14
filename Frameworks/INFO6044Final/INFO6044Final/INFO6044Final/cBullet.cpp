#include "cBullet.h"
#include "globals.h"
#include <iostream>

cBullet::cBullet(int _ownerID, glm::vec3 _position, glm::vec3 _direction) {
	ownerID = _ownerID;
	direction = _direction;
	model = new cMesh();
	model->meshName = "Sphere_xyz_n_rgba.ply";
	model->positionXYZ = _position;
	model->scale = 0.25f;
	model->bUseWholeObjectDiffuseColour = true;
	model->wholeObjectDiffuseRGBA = glm::vec4(0.f, 0.f, 0.f, 1.f);
	::g_vec_pMeshes.push_back(model);
}

cBullet::~cBullet() {
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

void cBullet::TimeStep(float deltaTime) {
	// Unlike the tanks, the bullet moves first and asks questions later. This will allow it to show collisions.
	model->positionXYZ += glm::vec3(direction.x * deltaTime * speed, direction.y * deltaTime * speed, direction.z * deltaTime * speed);
	if (!CheckValidMove()) {
		//We hit something. Mediator already knows and should clean this up.
		std::cout << "Unhandled collision. Bullet is still active." << std::endl;
	}
}

bool cBullet::RecieveMessage(sMessage theMessage) {

	return true;
}

bool cBullet::RecieveMessage(sMessage theMessage, sMessage& theResponse) {

	return true;
}

bool cBullet::SetReciever(iMediator* pTheReciever) {
	this->p_Mediator = pTheReciever;
	return true;
}

bool cBullet::CheckValidMove() {
	if (p_Mediator) {
		bool result = true;
		sMessage outgoingMessage;
		sMessage responseMessage;
		outgoingMessage.command = "VALIDATE BULLET HIT";
		outgoingMessage.vec_v4Data.push_back(glm::vec4(model->positionXYZ.x, model->positionXYZ.y, model->positionXYZ.z, 1.f));
		outgoingMessage.vec_iData.push_back(ownerID);
		outgoingMessage.vec_fData.push_back(damage);
		result &= p_Mediator->RecieveMessage(outgoingMessage, responseMessage);
		if (responseMessage.command == "CLEAR") {
			outgoingMessage.command = "VALIDATE BULLET POSITION";
			outgoingMessage.vec_v4Data.push_back(glm::vec4(model->positionXYZ.x, model->positionXYZ.y, model->positionXYZ.z, 1.f));
			outgoingMessage.vec_iData.push_back(ownerID);
			result &= p_Mediator->RecieveMessage(outgoingMessage);
		}
		return result;
	}
	else {
		// Flying blind. Better not move until someone is directing traffic.
		return false;
	}

}