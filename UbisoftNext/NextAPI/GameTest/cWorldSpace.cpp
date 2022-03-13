#include "stdafx.h"
#include "cWorldSpace.h"
#include "app\app.h"

cWorldSpace* cWorldSpace::_instance = 0;
cWorldSpace::cWorldSpace() {

}

void cWorldSpace::Init() {
	drawOffset.x = APP_VIRTUAL_WIDTH / 2.f;
	drawOffset.y = APP_VIRTUAL_HEIGHT / 2.f;
	cameraBounds.x = APP_VIRTUAL_WIDTH / 3.f;
	cameraBounds.y = APP_VIRTUAL_HEIGHT / 3.f;

	player = new cPlayer(0.f, 0.f, 1.f);
	planet = new cPlanet(0.f * scale, -500.f * scale, 225.f);
}

void cWorldSpace::Cleanup() {
	if (player) {
		delete player;
		player = 0;
	}
	if (planet) {
		delete planet;
		planet = 0;
	}
}

cWorldSpace* cWorldSpace::Instance() {
	if (_instance == 0) {
		_instance = new cWorldSpace;
	}

	return _instance;
}

void cWorldSpace::Draw() {
	player->Draw();
	planet->Draw();
}

void cWorldSpace::Update(float deltaTime) {
	player->Update(deltaTime);

	Vec2 playerPos = player->GetPosition();

	// Update the scale
	scale = GetUpdatedScale();
	player->SetScale(player->GetScale());

	// Handle world boundaries.
	if (playerPos.x > worldUpperBounds.x *scale) {
		playerPos.x = worldUpperBounds.x * scale;
		player->SetPosition(playerPos.x, playerPos.y);
	}
	if (playerPos.x < worldLowerBounds.x * scale) {
		playerPos.x = worldLowerBounds.x * scale;
		player->SetPosition(playerPos.x, playerPos.y);
	}
	if (playerPos.y > worldUpperBounds.y * scale) {
		playerPos.y = worldUpperBounds.y * scale;
		player->SetPosition(playerPos.x, playerPos.y);
	}
	if (playerPos.y < worldLowerBounds.y * scale) {
		playerPos.y = worldLowerBounds.y * scale;
		player->SetPosition(playerPos.x, playerPos.y);
	}

	// Handle camera changes.
	if (playerPos.x > (cameraPosition.x + cameraBounds.x)) {
		//Player has moved to the right of our boundary. Adjust.
		cameraPosition.x +=  playerPos.x - (cameraPosition.x + cameraBounds.x);
	}
	if (playerPos.x < (cameraPosition.x - cameraBounds.x)) {
		//Player has moved to the right of our boundary. Adjust.
		cameraPosition.x -= (cameraPosition.x - cameraBounds.x) - playerPos.x;
	}
	if (playerPos.y > (cameraPosition.y + cameraBounds.y)) {
		//Player has moved to the right of our boundary. Adjust.
		cameraPosition.y += playerPos.y - (cameraPosition.y + cameraBounds.y);
	}
	if (playerPos.y < (cameraPosition.y - cameraBounds.y)) {
		//Player has moved to the right of our boundary. Adjust.
		cameraPosition.y -= (cameraPosition.y - cameraBounds.y) - playerPos.y;
	}

	//Check for crash. Eventually handle in more scalable manner.
	if (planet->CheckForCrash(player->GetPosition(), 10.f)) {
		player->SetPosition(-cameraPosition.x, -cameraPosition.y);
	}
}

float cWorldSpace::GetScale() {
	return scale;
}

Vec2 cWorldSpace::GetCameraPosition() {
	return cameraPosition;
}

void cWorldSpace::GetCameraPosition(float& x, float& y) {
	x = cameraPosition.x;
	y = cameraPosition.y;
}

void cWorldSpace::SetCameraPosition(Vec2 _pos) {
	cameraPosition = _pos;
}

void cWorldSpace::SetCameraPosition(float _posX, float _posY) {
	cameraPosition.x = _posX;
	cameraPosition.y = _posY;
}

Vec2 cWorldSpace::GetDrawPosition() {
	Vec2 result;
	result.x = drawOffset.x - cameraPosition.x;
	result.y = drawOffset.y - cameraPosition.y;

	return result;
}

void cWorldSpace::GetDrawPosition(float& x, float& y) {
	x = drawOffset.x - cameraPosition.x;
	y = drawOffset.y - cameraPosition.y;
}

float cWorldSpace::GetUpdatedScale() {
	Vec2 playerPos = player->GetPosition();
	Vec2 planetPos = planet->GetPosition();
	float planetRadius = planet->GetRadius();

	float diff_x = playerPos.x - planetPos.x;
	float diff_y = playerPos.y - planetPos.y;
	float distance = sqrt(pow(diff_x,2) + pow(diff_y,2));
	

	//First test upper and lower bounds, then if in the middle, return appropriate zoom level.
	if (distance < planetRadius * 2.5f) {
		// They are either inside the planet or quite close to it.
		return maxScale;
	}
	else if (distance > planetRadius * 4.f) {
		// They are far away from the planet.
		return minScale;
	}

	//This math could be simplified but am leaving in case we wish to make the min max distances into variables rather than 'magic' numbers.
	float result =  minScale + ((distance - planetRadius *4.f) / (planetRadius*2.5f - planetRadius*4.f) * (maxScale - minScale));
	return result;
}

void HandleCrash() {
	//TODO : Move check to mediator.

}