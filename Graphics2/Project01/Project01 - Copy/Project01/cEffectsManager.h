#pragma once
#include <string>
#include <vector>

class cEffectsManager {
public:
	void Initialize();
	std::string GetStaticTexture();
private:
	std::vector<std::string> staticTextures;
};