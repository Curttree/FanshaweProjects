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
}

void cWorldSpace::Cleanup() {
	if (player) {
		delete player;
		player = 0;
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
}

void cWorldSpace::Update(float deltaTime) {
	player->Update(deltaTime);

	//Handle camera changes.

	Vec2 playerPos = player->GetPosition();
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
}

float cWorldSpace::GetScale() {
	return scale;
}
void cWorldSpace::SetScale(float _scale) {
	scale = _scale;
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