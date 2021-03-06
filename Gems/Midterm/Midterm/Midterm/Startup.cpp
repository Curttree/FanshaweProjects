#include "globals.h"

bool g_StartUp(GLFWwindow* pWindow)
{
	srand(time(0));

	::g_pFlyCamera = new cFlyCamera();

	::g_pShaderManager = new cShaderManager();

	::g_pTheLights = new cLightManager();

	::g_pVAOManager = new cVAOManager();

	::g_pTextureManager = new cBasicTextureManager();

	::g_pmazeMaker = new cMazeMaker();

	::g_pEcoSystemManager = new cEcosystemManager();

	return true;
}
