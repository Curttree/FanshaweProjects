#pragma once
#include <vector>
#include "GameEvents.h"
#include <cEntity.h>
#include "UIState.h"

class UIManager : public iGameEventListener{
public:
	static UIManager* Instance();
	void Initialize();
	void TimeStep(float deltaTime);
	//via iGameEventListener
	virtual void Notify(GameEventType type, void* data);
	//Can be made private/ otherwise abstracted.
private:
	static UIManager* _instance;
	UIManager();
	UIState currentState = UIState::WAITING;
	
	float delayTime = 0.f;
	float delayCounter = 0.f;

	cEntity* title;
	std::vector<cEntity*> canIcons;
	
	float displayTime = 0.f;
	float displayCounter = 0.f;
	bool showCans = false;
};