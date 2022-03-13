#pragma once
#include "sVec2.h"
#include "Entities/cPlayer.h"

class cWorldSpace {
public:
	static cWorldSpace* Instance();
	float GetScale();
	void SetScale(float _scale);
	void Init();
	void Cleanup();
	void Draw();
	void Update(float deltaTime);

	Vec2 GetCameraPosition();
	void GetCameraPosition(float& x, float& y);
	void SetCameraPosition(Vec2 _pos);
	void SetCameraPosition(float _posX, float _posY);

	Vec2 GetDrawPosition();
	void GetDrawPosition(float& x, float& y);

	cPlayer* player;
private:
	cWorldSpace();
	static cWorldSpace* _instance;
	float scale = 1.f;
	Vec2 cameraPosition = { 0.f,0.f };
	Vec2 drawOffset = { 0.f,0.f };
	Vec2 cameraOffset = { 0.f,0.f };
	Vec2 cameraBounds = { 200.f,200.f };
	Vec2 worldBounds = { 2000.f,2000.f };
};