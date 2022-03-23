#include "stdafx.h"
#include "cGameStateController.h"

void cGameStateController::Update(float deltaTime) {
	fuel -= deltaTime * fuelConsumption;
}

int cGameStateController::GetLives() {
	return lives;
}

int cGameStateController::GetScore() {
	return score;
}

float cGameStateController::GetFuel() {
	return fuel;
}

void cGameStateController::IncrementLives(int change) {
	lives += change;
}
void cGameStateController::IncrementScore(int change) {
	score += change;
}
void cGameStateController::IncrementFuel(float change) {
	fuel += change;
}
void cGameStateController::ResetFuel() {
	fuel = initialFuel;
}