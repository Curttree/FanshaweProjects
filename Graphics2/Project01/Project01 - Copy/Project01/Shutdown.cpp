#include "globals.h"

bool g_ShutDown(GLFWwindow* pWindow)
{

	delete ::g_pShaderManager;

	delete ::g_pVAOManager;

	delete ::g_pTheLights;

	delete ::g_pFlyCamera;
	delete ::g_pTVCamera;

	delete ::g_pTextureManager;

	delete ::g_pConfigManager;

	delete ::g_pEffectsManager;

	return true;
}