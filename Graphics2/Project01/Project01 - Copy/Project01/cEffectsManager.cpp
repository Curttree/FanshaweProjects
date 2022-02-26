#include "cEffectsManager.h"
#include "globals.h"

cMathHelper* _mathHelper = cMathHelper::Instance();

void cEffectsManager::Initialize() {
	::g_pTextureManager->SetBasePath("assets/textures/Static");
	::g_pTextureManager->Create2DTextureFromBMPFile("1.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("2.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("3.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("4.bmp", true);
	staticTextures = { "1.bmp", "2.bmp", "3.bmp","4.bmp" };
}


std::string cEffectsManager::GetStaticTexture() {
	int result = rand() % (this->staticTextures.size() + 1);
	if (result >= this->staticTextures.size())
	{
		result = 0;
	}
	return staticTextures[result];
}