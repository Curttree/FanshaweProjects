#pragma once
#include "sVec2.h"
#include "Entities/cPlayer.h"
#include "Entities/cPlanet.h"

class cWorldSpace {
public:
	static cWorldSpace* Instance();
	float GetScale();
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
	cPlanet* planet;
private:
	cWorldSpace();
	static cWorldSpace* _instance;
	float GetUpdatedScale();

	float scale = 1.f;

	float minScale = 1.f;
	float maxScale = 1.5f;

	Vec2 cameraPosition = { 0.f,0.f };
	Vec2 drawOffset = { 0.f,0.f };
	Vec2 cameraOffset = { 0.f,0.f };
	Vec2 cameraBounds = { 200.f,200.f };
	Vec2 worldUpperBounds = { 1000.f,200.f };
	Vec2 worldLowerBounds = { -1000.f,-1200.f };
};