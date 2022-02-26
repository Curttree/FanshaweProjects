#include "globals.h"

bool g_StartUp(GLFWwindow* pWindow)
{

	::g_pFlyCamera = new cFlyCamera();
	::g_pActiveCamera = g_pFlyCamera;
	::g_pTVCamera = new cFlyCamera();

	::g_pShaderManager = new cShaderManager();

	::g_pTheLights = new cLightManager();

	::g_pVAOManager = new cVAOManager();

	::g_pTextureManager = new cBasicTextureManager();

	::g_pConfigManager = new configManager();

	::g_pEffectsManager = new cEffectsManager();

	return true;
}
