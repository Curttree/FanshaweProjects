#include "globals.h"

bool g_StartUp(GLFWwindow* pWindow)
{
	::g_pGameEngine = new cGameEngine();

	::g_pFlyCamera = new cFlyCamera();

	::g_pShaderManager = new cShaderManager();

	::g_pTheLights = new cLightManager();

	::g_pVAOManager = new cVAOManager();

	::g_pTextureManager = new cBasicTextureManager();

	return true;
}
