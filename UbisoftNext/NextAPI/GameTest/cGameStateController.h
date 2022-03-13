#pragma once
class cGameStateController {
public:
	void Update(float deltaTime);
	int GetLives();
	int GetScore();
	float GetFuel();
	void IncrementLives(int change);
	void IncrementScore(int change);
	void IncrementFuel(float change);
	void ResetFuel();
private:
	int lives = 3;
	int score = 0;
	float fuel = 1000.f;
	float initialFuel = 1000.f;
	float fuelConsumption = 0.01f;
};