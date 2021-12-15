#include "cAlien.h"
#include "globals.h"

cAlien::cAlien(int type)  {
	mesh = new cMesh();
	mesh->bDontLight = true;
	mesh->bUseWholeObjectDiffuseColour = true;
	mesh->wholeObjectSpecularRGB = glm::vec3(1.f, 1.f, 1.f);
	mesh->wholeObjectDiffuseRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
	mesh->friendlyName = "Alien";
	mesh->scale = 1.f;
	switch (type) {
		case(1): {
			pose1 = "Invaders/SpaceInvader_I1_block_pose_1.ply";
			pose2 = "Invaders/SpaceInvader_I1_block_pose_2.ply";
			break;
		}
		case(2): {
			pose1 = "Invaders/SpaceInvader_I2_block_pose_1.ply";
			pose2 = "Invaders/SpaceInvader_I2_block_pose_2.ply";
			break;
		}
		case(3): {
			pose1 = "Invaders/SpaceInvader_I3_block_pose_1.ply";
			pose2 = "Invaders/SpaceInvader_I3_block_pose_2.ply";
			break;
		}
		default: {
			break;
		}
	}
	mesh->meshName = pose1;  

	particleScale = 100.f;
	glm::vec3 position = glm::vec3(mesh->positionXYZ.x, mesh->positionXYZ.y + particleScale, mesh->positionXYZ.z);
	particle = new cParticle(position, 1.f, particleScale);
	particle->SetDamping(0.9f);
	particle->type = "Alien";
	particle->owner = this;
	worldSpace->_world->AddParticle(particle);
	debugMesh = new cMesh();
	debugMesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
	debugMesh->bIsWireframe = true;
	debugMesh->bUseObjectDebugColour = true;
	debugMesh->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	debugMesh->bDontLight = true;
	debugMesh->scale = particleScale;
	debugMesh->positionXYZ = mesh->positionXYZ;
}

cAlien::cAlien(std::string _pose1, std::string _pose2) {
	// For use if we want to define our own type.
}

cAlien::~cAlien() {

}

void cAlien::DelaySinceLastMove(float offset) {
	sinceLastMove -= offset;
}

void cAlien::Update(float deltaTime) {
	if (isDead) {
		if (mesh->scale > 0.f) {
			currentDeathTime += deltaTime;
			if (currentDeathTime >= deathTimer) {
				// We should properly dispose of the object, but for now, just hide them.
				mesh->scale = 0.f;
				currentDeathTime = 0.f;
			}
		}
	}
	else {
		sinceLastMove += deltaTime;
		if (sinceLastMove >= moveTimer) {
			particle->SetPosition(particle->GetPosition() + glm::vec3(direction * 10.f, 0.f, 0.f));
			if (isFirstPose) {
				mesh->meshName = pose2;
			}
			else {
				mesh->meshName = pose1;
			}
			isFirstPose = !isFirstPose;
			sinceLastMove = 0.f;
		}
		cEntity::Update(deltaTime);
		if (mesh->positionXYZ.x > 950.f || mesh->positionXYZ.x < -950.f) {

			sMessage collisionMessage;
			collisionMessage.command = "Reverse Ships";

			::g_pInvaderMediator->RecieveMessage(collisionMessage);
		}
	}
}



bool cAlien::RecieveMessage(sMessage theMessage) {
	if (theMessage.command == "Destroy") {
		isDead = true;
		mesh->meshName = "Invaders/SpaceInvader_Explosions.ply";
		worldSpace->_world->RemoveParticle(particle);
	}
	else if (theMessage.command == "Reverse Ships") {
		if (particle->GetPosition().y > -750.f) {
			particle->SetPosition(particle->GetPosition() + glm::vec3(direction * -10.f, -150.f, 0.f));
			direction *= -1;
		}
		else {
			if (!::g_pGameState->GameEnded() && !isDead) {
				sMessage gameOver;
				gameOver.command = "Game Over";

				::g_pInvaderMediator->RecieveMessage(gameOver);
			}
		}
	}
	else if (theMessage.command == "Speed Up Ships") {
		moveTimer -= 0.0075f;
		if (direction > 0.f) {
			direction+= 0.05f;
		}
		else {
			direction-= 0.05f;
		}
	}
	return true;
}
bool cAlien::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	return true;
}
bool cAlien::SetReciever(iMediator* pTheReciever) {
	return true;
}