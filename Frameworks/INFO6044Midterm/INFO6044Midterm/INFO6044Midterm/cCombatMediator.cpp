#include "cCombatMediator.h"
#include "cMesh.h"
#include "cPlayerTank.h"
#include "cAITank.h"
#include "globals.h"

cCombatMediator::cCombatMediator() {
	mazeManager = new cMazeManager(); 
}

cCombatMediator::~cCombatMediator() {
	if (mazeManager) {
		delete mazeManager;
		mazeManager = NULL;
	}
}

bool cCombatMediator::RecieveMessage(sMessage theMessage) {
	if (theMessage.command == "INITIALIZE GAME") {
		mazeManager->LoadMazeFromFile(theMessage.vec_sData[0]);
		InitializeTanks();
	}
	else if (theMessage.command == "TIME STEP") {
		TimeStep(theMessage.vec_fData[0]);
	}
	else if (theMessage.command == "VALIDATE BULLET POSITION") {
		ValidateBulletPosition(theMessage.vec_iData[0], theMessage.vec_v4Data[0]);
	}
	else if (theMessage.command == "FIRE BULLET" && theMessage.vec_iData[0] == ::g_pPlayerTank->GetId()) {
		// Forward message so we can track its origin through the mediator.
		::g_pPlayerTank->RecieveMessage(theMessage);
	}
	return true;
}

bool cCombatMediator::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	if (theMessage.command == "VALIDATE MOVE") {
		// VALIDATE ABSOLUTE BOUNDS OF THE BOARD IN CASE A WALL IS BYPASSED
		if (theMessage.vec_v4Data[0].x > 60 || theMessage.vec_v4Data[0].x < 0
			|| theMessage.vec_v4Data[0].y > 100 || theMessage.vec_v4Data[0].y < 0) {
			theResponse.command = "RETURN TO BOARD";
		}
		else if (!mazeManager->ValidatePosition(theMessage.vec_v4Data[0], theMessage.vec_v4Data[1])) {
			// On the board, but the maze manager decided you can't move there.
			theResponse.command = "DON'T MOVE";
		}
		else {
			theResponse.command = "MOVE";
		}
	}
	else if (theMessage.command == "CHECK SIGHT") {
		for (iTank* tank : tanks) {
			if (tank->GetId() == theMessage.vec_iData[0]) {
				ValidateSight(tank, theResponse);
				break;
			}
		}
	}

	return true;
}

void cCombatMediator::ValidateBulletPosition(int ownerID, glm::vec3 position) {
	if (position.x > 60 || position.x < 0 || position.y > 100 || position.y < 0) {
		// Out of bounds
		CleanupBullet(ownerID);
	}
	else if (!mazeManager->ValidatePosition(position, glm::vec3(0.f))) {
		// Hit a wall
		CleanupBullet(ownerID);
	}
}

void cCombatMediator::CleanupBullet(int ownerID) {
	// Know there is a more scalable way to do this, but I am pressed for time and number of tanks is small.
	for (iTank* tank : tanks) {
		if (tank->GetId() == ownerID) {
			sMessage bulletMessage;
			bulletMessage.command = "DESTROY BULLET";
			tank->RecieveMessage(bulletMessage);
			break;
		}
	}
}

void cCombatMediator::ValidateSight(iTank* observer, sMessage& response) {
	glm::vec3 heading = observer->GetHeading();
	glm::vec3 position = observer->GetPosition();

	for (iTank* otherTank : tanks) {
		if (otherTank->GetId() != observer->GetId()) {
			//facing left
			if (heading.x > 0 && otherTank->GetPosition().y == position.y && otherTank->GetPosition().x > position.x) {
				response.vec_iData.push_back(otherTank->GetId());
			}
			//facing right
			if (heading.x < 0 && otherTank->GetPosition().y == position.y && otherTank->GetPosition().x < position.x) {
				response.vec_iData.push_back(otherTank->GetId());
			}
			//facing up
			if (heading.y > 0 && otherTank->GetPosition().x == position.x && otherTank->GetPosition().y > position.y) {
				response.vec_iData.push_back(otherTank->GetId());
			}
			//facing down
			if (heading.y < 0 && otherTank->GetPosition().x == position.x && otherTank->GetPosition().y < position.y) {
				response.vec_iData.push_back(otherTank->GetId());
			}
		}
	}
	if (response.vec_iData.size() > 0) {
		response.command = "SPOTTED";
	}
	else {
		response.command = "CLEAR";
	}
}

bool cCombatMediator::SetReciever(iMediator* pTheReciever) {
	// Doesn't do anything. This is the mediator.
	return true;
}

void cCombatMediator::TimeStep(float deltaTime) {
	for (int x = 0; x < tanks.size(); x++) {
		tanks[x]->TimeStep(deltaTime);
	}
}

void cCombatMediator::InitializeTanks() {
	// Hardcoded starting zone values
	float x1 = 14.f;
	float x2 = 44.f;
	float y1 = 14.f;
	float y2 = 50.f;
	float y3 = 85.f;

	// Forgoing Builder/Factory as we are creating only a few tanks that are almost identical, and have almost identical configurations. Would implement these patterns if variety was greater.

	cMesh* playerTankMesh = new cMesh();
	playerTankMesh->meshName = "Low Poly Tank Model 3D model.ply";
	playerTankMesh->positionXYZ = glm::vec3(x1,y2, 20.f);
	playerTankMesh->orientationXYZ = glm::vec3(-glm::pi<float>() / 2, 0.f, 0.f);
	playerTankMesh->scale = 0.25f;
	playerTankMesh->bUseWholeObjectDiffuseColour = false;
	::g_vec_pMeshes.push_back(playerTankMesh);
	g_pPlayerTank = new cPlayerTank(playerTankMesh);
	g_pPlayerTank->SetReciever(this);
	tanks.push_back(g_pPlayerTank);

	cMesh* blueTankMesh = new cMesh();
	blueTankMesh->meshName = "Low Poly Tank Model 3D model.ply";
	blueTankMesh->positionXYZ = glm::vec3(x2, y1, 20.f);
	blueTankMesh->orientationXYZ = glm::vec3(-glm::pi<float>() / 2, 0.f, 0.f);
	blueTankMesh->scale = 0.25f;
	blueTankMesh->bUseWholeObjectDiffuseColour = true;
	blueTankMesh->wholeObjectDiffuseRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
	::g_vec_pMeshes.push_back(blueTankMesh);
	iTank* blueTank = new cAITank(blueTankMesh);
	blueTank->SetReciever(this);
	tanks.push_back(blueTank);

	cMesh* redTankMesh = new cMesh();
	redTankMesh->meshName = "Low Poly Tank Model 3D model.ply";
	redTankMesh->positionXYZ = glm::vec3(x1, y1, 20.f);
	redTankMesh->orientationXYZ = glm::vec3(-glm::pi<float>() / 2, 0.f, 0.f);
	redTankMesh->scale = 0.25f;
	redTankMesh->bUseWholeObjectDiffuseColour = true;
	redTankMesh->wholeObjectDiffuseRGBA = glm::vec4(1.f, 0.f, 0.f, 1.f);
	::g_vec_pMeshes.push_back(redTankMesh);
	iTank* redTank = new cAITank(redTankMesh);
	redTank->SetReciever(this);
	tanks.push_back(redTank);

	cMesh* yellowTankMesh = new cMesh();
	yellowTankMesh->meshName = "Low Poly Tank Model 3D model.ply";
	yellowTankMesh->positionXYZ = glm::vec3(x2, y2, 20.f);
	yellowTankMesh->orientationXYZ = glm::vec3(-glm::pi<float>() / 2, 0.f, 0.f);
	yellowTankMesh->scale = 0.25f;
	yellowTankMesh->bUseWholeObjectDiffuseColour = true;
	yellowTankMesh->wholeObjectDiffuseRGBA = glm::vec4(1.f, 1.f, 0.f, 1.f);
	::g_vec_pMeshes.push_back(yellowTankMesh);
	iTank* yellowTank = new cAITank(yellowTankMesh);
	yellowTank->SetReciever(this);
	tanks.push_back(yellowTank);

	cMesh* magentaTankMesh = new cMesh();
	magentaTankMesh->meshName = "Low Poly Tank Model 3D model.ply";
	magentaTankMesh->positionXYZ = glm::vec3(x1, y3, 20.f);
	magentaTankMesh->orientationXYZ = glm::vec3(-glm::pi<float>() / 2, 0.f, 0.f);
	magentaTankMesh->scale = 0.25f;
	magentaTankMesh->bUseWholeObjectDiffuseColour = true;
	magentaTankMesh->wholeObjectDiffuseRGBA = glm::vec4(1.f, 0.f, 1.f, 1.f);
	::g_vec_pMeshes.push_back(magentaTankMesh);
	iTank* magentaTank = new cAITank(magentaTankMesh);
	magentaTank->SetReciever(this);
	tanks.push_back(magentaTank);

	cMesh* cyanTankMesh = new cMesh();
	cyanTankMesh->meshName = "Low Poly Tank Model 3D model.ply";
	cyanTankMesh->positionXYZ = glm::vec3(x2, y3, 20.f);
	cyanTankMesh->orientationXYZ = glm::vec3(-glm::pi<float>() / 2, 0.f, 0.f);
	cyanTankMesh->scale = 0.25f;
	cyanTankMesh->bUseWholeObjectDiffuseColour = true;
	cyanTankMesh->wholeObjectDiffuseRGBA = glm::vec4(0.f, 1.f, 1.f, 1.f);
	::g_vec_pMeshes.push_back(cyanTankMesh);
	iTank* cyanTank = new cAITank(cyanTankMesh);
	cyanTank->SetReciever(this);
	tanks.push_back(cyanTank);
}