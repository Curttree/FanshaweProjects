#pragma once
#include <string>
#include <vector>

class cEffectsManager {
public:
	void Initialize();
	void TimeStep(float deltaTime);
	std::string GetStaticTexture(unsigned int screen);
	float GetFlickerAmount(unsigned int screen);

	//Not really an effect, but store it here since it sort of relates.
	unsigned int GetCameraNumber(unsigned int screen);
private:
	std::vector<std::string> staticTextures;
	std::vector<std::string> screenTextures;

	float timeToStatic1 = 0.f;
	float staticTimer1 = 0.f;
	bool showStatic1 = false;
	unsigned int screen1ID = 0;

	float timeToStatic2 = 0.f;
	float staticTimer2 = 0.f;
	bool showStatic2 = false;
	unsigned int screen2ID = 0;


	float timeToFlicker1 = 0.f;
	float flickerTimer1 = 0.f;
	bool showFlicker1 = false;
	float flicker1Value = 0.f;
	float flicker1MaxValue = 0.f;

	float timeToFlicker2 = 0.f;
	float flickerTimer2 = 0.f;
	bool showFlicker2 = false;
	float flicker2Value = 0.f;
	float flicker2MaxValue = 0.f;


	float timeToChange1 = 0.f;
	float changeTimer1 = 0.f;
	unsigned int outsideScreen1ID = 0;
	unsigned int showing1 = 0;

	float timeToChange2 = 0.f;
	float changeTimer2 = 0.f;
	unsigned int outsideScreen2ID = 0;
	unsigned int showing2 = 0;


	float timeToChange3 = 0.f;
	float changeTimer3 = 0.f;
	unsigned int showing3 = 0;

	float timeToChange4 = 0.f;
	float changeTimer4 = 0.f;
	unsigned int showing4 = 0;
};