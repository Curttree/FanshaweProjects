#include "globals.h"

bool g_ShutDown(GLFWwindow* pWindow)
{
	delete ::g_pFlyCamera;

	::g_pGameEngine->Destroy();

	delete ::g_pGameEngine;

	return true;
}