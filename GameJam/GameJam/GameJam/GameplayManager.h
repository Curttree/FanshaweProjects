#pragma once

#include <vector>
#include <cEntity.h>


class GameplayManager {
public:
	GameplayManager();
	virtual ~GameplayManager();
	std::vector<cEntity*> canList;
	void SetupScene(void);
	void GameStart();
	void GameOver();
	void SetAiming(bool _aiming);
	bool GetAiming();
	//void Fire();
	
private:
	int cansRemaining = 1;
	bool gameStarted = false;
	bool aiming = false;
};