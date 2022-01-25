#include "globals.h"

bool g_StartUp(GLFWwindow* pWindow)
{

	::g_pFlyCamera = new cFlyCamera();
	
	::g_pGameEngine = new cGameEngine();

	return ::g_pGameEngine->Initialize();
}
