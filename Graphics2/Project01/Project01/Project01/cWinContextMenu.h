#ifndef _cWinContextMenu_HG_
#define _cWinContextMenu_HG_

#include "globals.h"

#ifdef Windows_Context_Menu_Usage_Custom

class cWinContextMenu
{
public:
	cWinContextMenu();

	void showMenu(GLFWwindow* window, int x, int y);

};

#endif

#endif

