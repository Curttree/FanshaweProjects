#include "cEffectsManager.h"
#include "globals.h"
#include "globalFunctions.h"

cMathHelper* _mathHelper = cMathHelper::Instance();

void cEffectsManager::TimeStep(float detlaTime) {

}

void cEffectsManager::Initialize() {
	::g_pTextureManager->SetBasePath("assets/textures/Static");
	::g_pTextureManager->Create2DTextureFromBMPFile("1.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("2.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("3.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("4.bmp", true);
	staticTextures = { "1.bmp", "2.bmp", "3.bmp","4.bmp" };
}


std::string cEffectsManager::GetStaticTexture() {
	int result = gGetRandBetween(0, (int)staticTextures.size());
	return staticTextures[result];
}