#pragma once

#include <vector>
#include "cProp.h"


class GameplayManager {
public:
	GameplayManager();
	virtual ~GameplayManager();
	std::vector<cProp*> canList;
	void SetupScene(void);
	void GameStart();
	void GameOver();
	void SetAiming(bool _aiming);
	bool GetAiming();
	void Fire();
	
private:
	int cansRemaining = 1;
	bool gameStarted = false;
	bool aiming = false;
};