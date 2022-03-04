#pragma once
#include <string>
#include <vector>

class cEffectsManager {
public:
	void Initialize();
	void TimeStep(float deltaTime);
	std::string GetStaticTexture(unsigned int screen);
	float GetFlickerAmount(unsigned int screen);
private:
	std::vector<std::string> staticTextures;

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
};