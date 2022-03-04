#pragma once
#include <string>
#include <vector>

class cEffectsManager {
public:
	void Initialize();
	void TimeStep(float deltaTime);
	std::string GetStaticTexture();
private:
	std::vector<std::string> staticTextures;
	float timeToStatic = 0.f;
	float staticTimer = 0.f;
	bool showStatic = false;
};