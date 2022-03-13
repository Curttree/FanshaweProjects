#include "stdafx.h"
#include "cWorldSpace.h"
#include "app\app.h"

//Forward declaration of global function.
float gComparePositions(Vec2 pos1, Vec2 pos2);

cWorldSpace* cWorldSpace::_instance = 0;
cWorldSpace::cWorldSpace() {

}

void cWorldSpace::Init() {
	drawOffset.x = APP_VIRTUAL_WIDTH / 2.f;
	drawOffset.y = APP_VIRTUAL_HEIGHT / 2.f;
	cameraBounds.x = APP_VIRTUAL_WIDTH / 3.f;
	cameraBounds.y = APP_VIRTUAL_HEIGHT / 3.f;

	gameState = new cGameStateController();

	player = new cPlayer(0.f,200.f, 1.f);
	planet = new cPlanet(0.f * scale, -500.f * scale, 225.f,1000000.f);
	spawner = new cSpawner(planet);

	spawner->Init();
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
	if (spawner) {
		delete spawner;
		spawner = 0;
	}
	if (gameState) {
		delete gameState;
		gameState = 0;
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
	for each (iProjectile* proj in projectiles)
	{
		proj->Draw();
	}
	for each (cGameEntity * structure in structures)
	{
		structure->Draw();
	}
}

void cWorldSpace::Update(float deltaTime) {
	// This could be split up into separate methods, or ideally, individually classes to handle different responsibilities. Will adjust if I have extra time.
	player->Update(deltaTime);
	planet->Update(deltaTime);
	spawner->Update(deltaTime);
	gameState->Update(deltaTime);
	UpdateProjectiles(deltaTime);
	UpdateStructures(deltaTime);

	Vec2 playerPos = player->GetPosition();
	Vec2 planetPos = planet->GetPosition();

	// Update the scale
	scale = GetUpdatedScale();
	player->SetScale(player->GetScale());

	// Apply gravity.
	float gravDirectionMagnitude = 0.f;
	float gravMagnitude = planet->CalculateGravity(player->GetPosition(), gravDirectionMagnitude);
	Vec2 gravDirection;
	gravDirection.x = (planetPos.x - playerPos.x) / gravDirectionMagnitude;
	gravDirection.y = (planetPos.y - playerPos.y) / gravDirectionMagnitude;
	player->Pull(gravDirection, gravMagnitude);

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
	if (planet->CheckForCrash(player->GetPosition(), player->GetRadius())) {
		HandleCrash();
	}

	HandleBulletSurfaceCollisions();
	HandleBulletEntityCollisions();
	HandlePlayerStructureCollisions();

	if (gameState->GetFuel() <= 0.f) {
		HandleCrash();
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

void cWorldSpace::UpdateStructures(float deltaTime) {
	//Manage structures
	{
		unsigned int size = structures.size();
		for (unsigned int i = 0; i < size; i++)
		{
			if (structures[i]->ShouldDestroy()) {
				delete structures[i];
				structures.erase(structures.begin() + i);
				i--;
				size--;
			}
		}
		for each (cStructure * structure in structures)
		{
			structure->Update(deltaTime);
		}
	}
}

void cWorldSpace::UpdateProjectiles(float deltaTime) {
	//Manage projectiles
	{
		unsigned int size = projectiles.size();
		for (unsigned int i = 0; i < size; i++)
		{
			if (projectiles[i]->ShouldDestroy()) {
				delete projectiles[i];
				projectiles.erase(projectiles.begin() + i);
				i--;
				size--;
			}
		}
		for each (iProjectile * proj in projectiles)
		{
			proj->Update(deltaTime);
		}
	}
}

void cWorldSpace::HandleCrash() {
	//TODO : Move check to mediator.
	player->SetPosition(0.f, 200.f);
	gameState->IncrementLives(-1);
	gameState->ResetFuel();
}

void cWorldSpace::HandleBulletSurfaceCollisions() {
	//First check to see if any bullets have hit the surface of our planet.
	for each (iProjectile* proj in projectiles)
	{
		if (planet->CheckForCrash(proj->GetPosition(), proj->GetRadius())) {
			proj->Destroy();
		}
	}
}

void cWorldSpace::HandleBulletEntityCollisions() {
	for each (iProjectile * proj in projectiles)
	{
		if (proj->GetOwner() == player) {
			//Compare with positions of buildings.
			// Complexity is not great here. See if we can refactor to improve performance.
			for each (cStructure * structure in structures) {
				if (gComparePositions(structure->GetPosition(), proj->GetPosition()) <= structure->GetRadius() + proj->GetRadius()) {
					proj->Destroy();
					structure->Shot();
				}
			}
		}
		else {
			//Compare with position of player.
			if (gComparePositions(player->GetPosition(), proj->GetPosition()) <= player->GetRadius() + proj->GetRadius()) {
				proj->Destroy();
				HandleCrash();
			}
		}
	}
}

void cWorldSpace::HandlePlayerStructureCollisions() {
	for each (cStructure * structure in structures) {
		if (gComparePositions(structure->GetPosition(), player->GetPosition()) <= structure->GetRadius() + player->GetRadius()) {
			structure->Crash();
		}
	}
}