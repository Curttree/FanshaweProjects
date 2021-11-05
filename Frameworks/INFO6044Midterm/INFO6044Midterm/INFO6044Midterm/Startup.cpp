#include "globals.h"
#include "cCombatMediator.h"
#include <time.h>

bool g_StartUp(GLFWwindow* pWindow)
{

	::g_pFlyCamera = new cFlyCamera();

	::g_pShaderManager = new cShaderManager();

	::g_pTheLights = new cLightManager();

	::g_pVAOManager = new cVAOManager();

	::g_pCombatMediator = new cCombatMediator(); 
	
	srand(static_cast <unsigned> (time(0)));

	return true;
}
