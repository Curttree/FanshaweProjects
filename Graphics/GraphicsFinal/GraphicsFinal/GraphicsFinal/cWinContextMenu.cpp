
#include "cWinContextMenu.h"

#ifdef Windows_Context_Menu_Usage_Custom

// Note: we *don't* need to (or should) #include Windows.h
// Instead, we ask GFLW to expose the Win32 calls. 
//      #include <Windows.h>

#include <iostream>

// The global.h only has a forward declaration, so we need to include the actual header
//#include "cMesh.h"

cWinContextMenu::cWinContextMenu()
{

}

void cWinContextMenu::showMenu(GLFWwindow* window, int x, int y)
{
    // Windows communication is mostly through "messages" that are passed around.
    // Things (mouse, packet, whatever) "send" messages to a queue, a "message pump"
    //  does some possible adjustments to them (for languages, power settings, whatever), 
    //  then they are sent to the message handler, which is a giant switch statement that
    //  the user writes. Seriously, that's usually what it is. Sometimes it's hidden in a 
    //  web of object oriented lies and nonsense to pretend like it's not just doing 
    //  whatever a giant switch statement does, but I digress. 
    // 
    // Anywhoo, since there's a lot of pre-defined messages, you have to be careful that
    //  your messages aren't overlapping.
    // Oh, I should mention that "messages" are just ints. Yeah. 
    // 
    // To help with this, each application can start their messages at WM_USER 
    //  (0x0400 or 1024). 
    // Note that this is specific to the application and to the window, actually.
    // Like different programs can have the same messages, and different 
    //  windows within that application can also have the same messages.
    // This is because the messages are sent to 
    const UINT_PTR ID_WIREFRAME = WM_USER + 1;
    const UINT_PTR ID_POSITION2 = WM_USER + 2;
    const UINT_PTR ID_POSITION3 = WM_USER + 3;
    const UINT_PTR ID_TOGGLECOLLISION = WM_USER + 4;
    const UINT_PTR ID_CLOSE = WM_USER + 5;


    HMENU hPopupMenu = CreatePopupMenu();
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_CLOSE, (LPCWSTR)L"Exit");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_WIREFRAME, (LPCWSTR)L"Toggle Wireframe");

    HWND hWnd = glfwGetWin32Window(window);

    SetForegroundWindow(hWnd);
    // Note: not actually a "bool", it's an int
    // Yeah, talk to Microsoft, not me.
    // Actually, don't do that because even a "bool" isn't actually a bool, it's also an int.
    // True story.

    // TOP and BOTTOM align is how the menu pops up, either above or below the mouse
    // RETURN CMD says to NOT send the choice to the window, but instead return it just here.
    BOOL choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, x, y, 0, hWnd, NULL);

    // Returns 0 if no choice made

    // Now process the choice we made with the mouse click
    switch (choice)
    {
    case ID_CLOSE:
        std::cout << "Picked \"Exit\" (ID_CLOSE)" << std::endl;

        break;
    case ID_WIREFRAME:
        std::cout << "Picked \"Toggle Wireframe\" (ID_POSITION1)" << std::endl;

        for (cMesh* mesh : g_vec_pMeshes) {
            mesh->bIsWireframe = !mesh->bIsWireframe;
        }
        break;
    case 0:
        std::cout << "Didn't make a choice (hit <esc>?)" << std::endl;
        break;

    default:
        std::cout << "Unknown value returned from menu" << std::endl;
    }

    return;
}


#endif


void ShowWindowsContextMenu(GLFWwindow* window, int button, int action, int mods)
{
#ifdef Windows_Context_Menu_Usage_Custom

    cWinContextMenu thePopUpMenu;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);

    int menux = xpos + (int)mx;
    int menuy = ypos + (int)my;

    // Note: this is a "blocking" call... 
    std::cout << "Showing menu...";
    thePopUpMenu.showMenu(window, menux, menuy);
    std::cout << "Done showing menu." << std::endl;

#endif	
    return;
}