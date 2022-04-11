#include "globals.h"

bool g_ShutDown(GLFWwindow* pWindow)
{
	::g_pGameEngine->Destroy();

	delete ::g_pGameEngine;

	delete ::g_pShaderManager;

	delete ::g_pVAOManager;

	delete ::g_pTheLights;

	delete ::g_pFlyCamera;

	delete ::g_pTextureManager;

	return true;
}