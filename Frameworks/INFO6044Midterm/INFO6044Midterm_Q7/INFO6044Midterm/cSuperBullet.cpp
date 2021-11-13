#include "cSuperBullet.h"

cSuperBullet::cSuperBullet(int _ownerID, glm::vec3 _position, glm::vec3 _direction, float damageModifier) : cBullet(_ownerID, _position, _direction, damageModifier){
	model->wholeObjectDiffuseRGBA = glm::vec4(1.f, 0.f, 0.f, 1.f);
}

void cSuperBullet::Redirect() {
	redirected = true;
	model->positionXYZ -= glm::vec3(direction.x * 0.15f * speed, direction.y * 0.15f * speed, direction.z * 0.15f * speed);
	direction = glm::vec3(direction.x * -1.f, direction.y * -1.f, direction.z * -1.f);
}

bool cSuperBullet::CheckValidMove() {
	if (p_Mediator) {
		bool result = true;
		sMessage outgoingMessage;
		sMessage responseMessage;
		outgoingMessage.command = "VALIDATE BULLET HIT";
		outgoingMessage.vec_v4Data.push_back(glm::vec4(model->positionXYZ.x, model->positionXYZ.y, model->positionXYZ.z, 1.f));
		outgoingMessage.vec_iData.push_back(ownerID);
		outgoingMessage.vec_fData.push_back(damage);
		outgoingMessage.vec_sData.push_back("SUPERBULLET");
		result &= p_Mediator->RecieveMessage(outgoingMessage, responseMessage);
		if (responseMessage.command == "CLEAR") {
			sMessage outgoingMessage2;
			outgoingMessage2.command = "VALIDATE BULLET POSITION";
			if (redirected) {
				outgoingMessage2.vec_sData.push_back("SUPERBULLET2");
			}
			else {
				outgoingMessage2.vec_sData.push_back("SUPERBULLET1");
			}
			outgoingMessage2.vec_v4Data.push_back(glm::vec4(model->positionXYZ.x, model->positionXYZ.y, model->positionXYZ.z, 1.f));
			outgoingMessage2.vec_iData.push_back(ownerID);
			result &= p_Mediator->RecieveMessage(outgoingMessage2);
		}
		return result;
	}
	else {
		// Flying blind. Better not move until someone is directing traffic.
		return false;
	}

}