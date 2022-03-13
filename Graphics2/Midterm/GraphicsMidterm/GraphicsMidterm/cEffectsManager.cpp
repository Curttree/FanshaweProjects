#include "cEffectsManager.h"
#include "globals.h"
#include "globalFunctions.h"

cMathHelper* _mathHelper = cMathHelper::Instance();

//QUESTION_3 AND QUESTION_4

void cEffectsManager::TimeStep(float deltaTime) {
	staticTimer1 += deltaTime;
	staticTimer2 += deltaTime;
	flickerTimer1 += deltaTime;
	flickerTimer2 += deltaTime;
	changeTimer1 += deltaTime;
	changeTimer2 += deltaTime;
	changeTimer3 += deltaTime;
	changeTimer4 += deltaTime;
	
	if (changeTimer1 >= timeToChange1) {
		changeTimer1 = 0.f;
		showing1 = ::gGetRandBetween(0, 5);
		while (showing1 == showing2) {
			showing1 = ::gGetRandBetween(0, 5);
		}
		timeToChange1 = ::gGetRandBetween(3.f, 4.f);
	}

	if (changeTimer2 >= timeToChange2) {
		changeTimer2 = 0.f;
		showing2 = ::gGetRandBetween(0, 5);
		while (showing1 == showing2) {
			showing2 = ::gGetRandBetween(0, 5);
		}
		timeToChange2 = ::gGetRandBetween(3.f, 4.f);
	}

	if (changeTimer3 >= timeToChange3) {
		changeTimer3 = 0.f;
		showing3 = ::gGetRandBetween(0, 5);
		while (showing3 == showing4) {
			showing3 = ::gGetRandBetween(0, 5);
		}
		::g_vec_pMonitorsQ4[0]->textureNames[0] = screenTextures[showing3];
		timeToChange3 = ::gGetRandBetween(3.f, 4.f);
	}

	if (changeTimer4 >= timeToChange4) {
		changeTimer4 = 0.f;
		showing4 = ::gGetRandBetween(0, 5);
		while (showing3 == showing4) {
			showing4 = ::gGetRandBetween(0, 5);
		}
		::g_vec_pMonitorsQ4[1]->textureNames[0] = screenTextures[showing4];
		timeToChange4 = ::gGetRandBetween(3.f, 4.f);
	}


	if (staticTimer1 >= timeToStatic1) {
		staticTimer1 = 0.f;
		showStatic1 = !showStatic1;
		if (showStatic1) {
			timeToStatic1 = ::gGetRandBetween(1.f, 2.f);
		}
		else {
			timeToStatic1 = ::gGetRandBetween(3.f, 4.f);
		}
	}

	if (staticTimer2 >= timeToStatic2) {
		staticTimer2 = 0.f;
		showStatic2 = !showStatic2;
		if (showStatic2) {
			timeToStatic2 = ::gGetRandBetween(1.f, 2.f);
		}
		else {
			timeToStatic2 = ::gGetRandBetween(3.f, 4.f);
		}
	}

	if (showFlicker1 && flicker1Value > 0.f) {
		flicker1Value -= (deltaTime / timeToFlicker1) * flicker1MaxValue;
		if (flicker1Value < 0.f) {
			flicker1Value = 0.f;
		}
	}
	if (showFlicker2 && flicker2Value > 0.f) {
		flicker2Value -= (deltaTime / timeToFlicker2) * flicker2MaxValue;
		if (flicker2Value < 0.f) {
			flicker2Value = 0.f;
		}
	}

	if (flickerTimer1 >= timeToFlicker1) {
		flickerTimer1 = 0.f;
		showFlicker1 = !showFlicker1;
		if (showFlicker1) {
			flicker1MaxValue = ::gGetRandBetween(0.3f, 0.7f);
			flicker1Value = flicker1MaxValue;
			timeToFlicker1 = ::gGetRandBetween(1.0f, 1.5f);
		}
		else {
			timeToStatic1 = ::gGetRandBetween(3.f, 4.f);
		}
	}

	if (flickerTimer2 >= timeToFlicker2) {
		flickerTimer2 = 0.f;
		showFlicker2 = !showFlicker2;
		if (showFlicker2) {
			flicker2MaxValue = ::gGetRandBetween(0.3f, 0.7f);
			flicker2Value = flicker2MaxValue;
			timeToFlicker2 = ::gGetRandBetween(1.0f, 1.5f);
		}
		else {
			timeToStatic2 = ::gGetRandBetween(3.f, 4.f);
		}
	}
}

void cEffectsManager::Initialize() {
	::g_pTextureManager->SetBasePath("assets/textures/Static");
	::g_pTextureManager->Create2DTextureFromBMPFile("1.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("2.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("3.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("4.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("0.bmp", true);
	staticTextures = { "1.bmp", "2.bmp", "3.bmp","4.bmp" };

	::g_pTextureManager->SetBasePath("assets/textures");
	::g_pTextureManager->Create2DTextureFromBMPFile("starwars.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("sci_fi.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("sci_fi2.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("sci_fi3.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("sci_fi4.bmp", true);
	screenTextures = { "starwars.bmp", "sci_fi.bmp", "sci_fi2.bmp","sci_fi3.bmp", "sci_fi4.bmp"};

	screen1ID = ::g_vec_pMonitorsQ4[0]->getUniqueID();
	screen2ID = ::g_vec_pMonitorsQ4[1]->getUniqueID();
	outsideScreen1ID = ::g_vec_pMonitorsQ3[0]->getUniqueID();
	outsideScreen2ID = ::g_vec_pMonitorsQ3[1]->getUniqueID();

	timeToStatic1 = ::gGetRandBetween(3.f,4.f);
	timeToStatic2 = ::gGetRandBetween(3.f, 4.f);
	timeToFlicker1 = ::gGetRandBetween(3.f, 4.f);
	timeToFlicker2 = ::gGetRandBetween(3.f, 4.f);


	showing1 = ::gGetRandBetween(0, 5);
	showing2 = ::gGetRandBetween(0, 5);
	timeToChange1 = ::gGetRandBetween(3.f, 4.f);
	timeToChange2 = ::gGetRandBetween(3.f, 4.f);

	showing3 = ::gGetRandBetween(0, 5);
	showing4 = ::gGetRandBetween(0, 5);
	timeToChange3 = ::gGetRandBetween(3.f, 4.f);
	timeToChange4 = ::gGetRandBetween(3.f, 4.f);
}


std::string cEffectsManager::GetStaticTexture(unsigned int screen) {
	if ((screen == screen1ID && showStatic1) || (screen == screen2ID && showStatic2)) {
			int result = gGetRandBetween(0, (int)staticTextures.size());
			return staticTextures[result];
	}
	else {
		return "0.bmp";
	}
}

float cEffectsManager::GetFlickerAmount(unsigned int screen) {
	if (screen == screen1ID) {
		return flicker1Value;
	}
	else if(screen == screen2ID) {
		return flicker2Value;
	}
	else {
		return 0.f;
	}
}

unsigned int cEffectsManager::GetCameraNumber(unsigned int screen) {
	if (screen == outsideScreen1ID) {
		return showing1;
	}
	else if (screen == outsideScreen2ID) {
		return showing2;
	}
	else {
		return 0;
	}
}