#include "stdafx.h"
#include "cGameEntity.h"
#include "..\cWorldSpace.h"

cWorldSpace* world = cWorldSpace::Instance();

void cGameEntity::Draw() {
	this->sprite->Draw();
}

void cGameEntity::Update(float deltaTime) {
	this->sprite->Update(deltaTime);
}

float cGameEntity::GetAngle() {
	return angle;
}

Vec2 cGameEntity::GetPosition() {
	return position;
}

void cGameEntity::GetPosition(float& x, float& y) {
	x = position.x;
	y = position.y;
}

float cGameEntity::GetScale() {
	return scale;
}

void cGameEntity::SetAngle(float _angle) {
	angle = _angle;
	this->sprite->SetAngle(_angle);
}
void cGameEntity::SetPosition(float _positionX, float _positionY) {
	position.x = _positionX;
	position.y = _positionY;
	Vec2 camera = world->GetDrawPosition();
	this->sprite->SetPosition(_positionX + camera.x, _positionY + camera.y);
}

void cGameEntity::SetScale(float _scale) {
	scale = _scale;
	this->sprite->SetScale(_scale * world->GetScale());
}