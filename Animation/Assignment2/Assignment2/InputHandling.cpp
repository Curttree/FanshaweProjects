#include "Graphics/GLCommon.h" 
#include "globals.h"

#include <sstream>
#include <iostream>

#include "cWinContextMenu.h"



// Handle async IO (keyboard, joystick, mouse, etc.)


// This is so the "fly camera" won't pay attention to the mouse if it's 
// not directly over the window. 
bool g_MouseIsInsideWindow = false;



void handleAsyncKeyboard(GLFWwindow* pWindow, double deltaTime)
{
    float cameraMoveSpeed = ::g_pFlyCamera->movementSpeed;

    float objectMovementSpeed = 5.f;
    float lightMovementSpeed = 10.0f;

    if ( cGFLWKeyboardModifiers::areAllModsUp(pWindow) )
    {
        // Use "fly" camera (keyboard for movement, mouse for aim)
        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS )
        {
            ::g_pFlyCamera->MoveForward_Z(+cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS )	// "backwards"
        {
            ::g_pFlyCamera->MoveForward_Z(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS )	// "left"
        {
            ::g_pFlyCamera->MoveLeftRight_X(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS )	// "right"
        {
            ::g_pFlyCamera->MoveLeftRight_X(+cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS )	// "up"
        {
            ::g_pFlyCamera->MoveUpDown_Y(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS )	// "down"
        {
            ::g_pFlyCamera->MoveUpDown_Y(+cameraMoveSpeed);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_KP_1) == GLFW_PRESS)
        {
            ::g_pGameEngine->SetGameSpeed(1.f);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_KP_2) == GLFW_PRESS)
        {
            ::g_pGameEngine->SetGameSpeed(2.f);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_3) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_KP_3) == GLFW_PRESS)
        {
            ::g_pGameEngine->SetGameSpeed(3.f);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_4) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_KP_4) == GLFW_PRESS)
        {
            ::g_pGameEngine->SetGameSpeed(4.f);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_5) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_KP_5) == GLFW_PRESS)
        {
            ::g_pGameEngine->SetGameSpeed(5.f);
        }
        //Use track camera controls.
        //if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS)
        //{
        //    ::g_pFlyCamera->Zoom(+cameraMoveSpeed);
        //}
        //if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        //{
        //    ::g_pFlyCamera->Zoom(-cameraMoveSpeed);
        //}
        //if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        //{
        //    ::g_pFlyCamera->RotateAlongTrack(-cameraMoveSpeed/10.f);
        //}
        //if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        //{
        //    ::g_pFlyCamera->RotateAlongTrack(cameraMoveSpeed/10.f);
        //}
        //if (glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_KP_1) == GLFW_PRESS)
        //{
        //    ::g_pGameEngine->SwapPlayer(0);
        //}
        //if (glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_KP_2) == GLFW_PRESS)
        //{
        //    ::g_pGameEngine->SwapPlayer(1);
        //}
        //if (glfwGetKey(pWindow, GLFW_KEY_3) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_KP_3) == GLFW_PRESS)
        //{
        //    ::g_pGameEngine->SwapPlayer(2);
        //}
        //if (glfwGetKey(pWindow, GLFW_KEY_4) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_KP_4) == GLFW_PRESS)
        //{
        //    ::g_pGameEngine->SwapPlayer(3);
        //}
        //if (glfwGetKey(pWindow, GLFW_KEY_5) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_KP_5) == GLFW_PRESS)
        //{
        //    ::g_pGameEngine->SwapPlayer(4);
        //}


        std::stringstream strTitle;
        // std::cout << 
        glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
        strTitle << "Camera: "
            << cameraEye.x << ", "
            << cameraEye.y << ", "
            << cameraEye.z; //<< std::endl;

        ::g_TitleText = strTitle.str();

    }//if ( cGFLWKeyboardModifiers::areAllModsUp(pWindow) )


//    // Basic camera controls (if NONE of the control keys are pressed)
//    if ( cGFLWKeyboardModifiers::areAllModsUp(pWindow) )
//    {
//        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS ) { ::g_cameraEye.x -= cameraMoveSpeed; } // Go left
//        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS ) { ::g_cameraEye.x += cameraMoveSpeed; } // Go right
//        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS ) { ::g_cameraEye.z += cameraMoveSpeed; }// Go forward 
//        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS ) { ::g_cameraEye.z -= cameraMoveSpeed; }// Go backwards
//        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS ) { ::g_cameraEye.y -= cameraMoveSpeed; }// Go "Down"
//        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS ) { ::g_cameraEye.y += cameraMoveSpeed; }// Go "Up"
//
//        std::stringstream strTitle;
//        // std::cout << 
//        strTitle << "Camera: "
//            << ::g_cameraEye.x << ", "
//            << ::g_cameraEye.y << ", "
//            << ::g_cameraEye.z; //<< std::endl;
//
//        ::g_TitleText = strTitle.str();
//
//    }//if ( areAllModsUp(window) )...

    // If JUST the ALT is down, move the "selected" light
    if ( cGFLWKeyboardModifiers::isModifierDown(pWindow, false, true, false) )
    {
        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS ) { ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].position.x -= lightMovementSpeed; } // Go left
        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS ) { ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].position.x += lightMovementSpeed; } // Go right
        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS ) { ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].position.z += lightMovementSpeed; }// Go forward 
        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS ) { ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].position.z -= lightMovementSpeed; }// Go backwards
        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS ) { ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].position.y -= lightMovementSpeed; }// Go "Down"
        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS ) { ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].position.y += lightMovementSpeed; }// Go "Up"

        // constant attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS )
        {
            ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.x *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.x <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.x = 0.001f;
            }
            ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.x *= 1.01f; // +1% more
        }
        // linear attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_3) == GLFW_PRESS )
        {
            ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.y *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_4) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.y <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.y = 0.001f;
            }
            ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.y *= 1.01f; // +1% more
        }
        // quadratic attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_5) == GLFW_PRESS )
        {
            ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.z *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_6) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.z <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.z = 0.001f;
            }
            ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.z *= 1.01f; // +1% more
        }

        if ( glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS ) { ::g_pGameEngine->g_pTheLights->theLights[0].param1.y -= 0.5f; }   // Inner
        if ( glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS ) { ::g_pGameEngine->g_pTheLights->theLights[0].param1.y += 0.5f; }   // Inner
        if ( glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS ) { ::g_pGameEngine->g_pTheLights->theLights[0].param1.z -= 0.5f; }   // Outer
        if ( glfwGetKey(pWindow, GLFW_KEY_P) == GLFW_PRESS ) { ::g_pGameEngine->g_pTheLights->theLights[0].param1.z += 0.5f; }   // Outer


        //std::stringstream strTitle;
        //// std::cout << 
        //strTitle << "Light # " << ::g_selectedLight << " positionXYZ : "
        //    << ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].position.x << ", "
        //    << ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].position.y << ", "
        //    << ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].position.z << "  "
        //    << "attenuation (C, L, Q): "
        //    << ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.x << ", "        // Const
        //    << ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.y << ", "        // Linear
        //    << ::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].atten.z << "  "        // Quadratic
        //    << (::g_pGameEngine->g_pTheLights->theLights[::g_selectedLight].param2.x > 0.0f ? " is on" : " is off");
        ////<< std::endl;


    }//if ( cGFLWKeyboardModifiers::...

    return;
}


// We call these every frame
void handleAsyncMouse(GLFWwindow* window, double deltaTime)
{

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    ::g_pFlyCamera->setMouseXY(x, y);

    const float MOUSE_SENSITIVITY = 2.0f;


    // Mouse left (primary?) button pressed? 
    // AND the mouse is inside the window...
    if ( (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        && ::g_MouseIsInsideWindow )
    {
        // Mouse button is down so turn the camera
        ::g_pFlyCamera->Yaw_LeftRight( ::g_pFlyCamera->getDeltaMouseX() * MOUSE_SENSITIVITY, deltaTime );

        ::g_pFlyCamera->Pitch_UpDown( -::g_pFlyCamera->getDeltaMouseY() * MOUSE_SENSITIVITY, deltaTime );

    }

    // Adjust the mouse speed
    if ( ::g_MouseIsInsideWindow )
    {
        const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

        // Adjust the movement speed based on the wheel position
        ::g_pFlyCamera->movementSpeed -= ( ::g_pFlyCamera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY );

        // Clear the mouse wheel delta (or it will increase constantly)
        ::g_pFlyCamera->clearMouseWheelValue();


        if ( ::g_pFlyCamera->movementSpeed <= 0.0f )
        {
            ::g_pFlyCamera->movementSpeed = 0.0f;
        }
    }


    return;
}

void GLFW_cursor_enter_callback(GLFWwindow* window, int entered)
{
    if ( entered )
    {
        std::cout << "Mouse cursor is over the window" << std::endl;
        ::g_MouseIsInsideWindow = true;
    }
    else
    {
        std::cout << "Mouse cursor is no longer over the window" << std::endl;
        ::g_MouseIsInsideWindow = false;
    }
    return;
}

// Called when the mouse scroll wheel is moved
void GLFW_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float mouseScrollWheelSensitivity = 0.1f;

    ::g_pFlyCamera->setMouseWheelDelta(yoffset * mouseScrollWheelSensitivity);

    return;
}

void GLFW_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

#ifdef YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG
    // Right button is pop-up
    if ( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS )
    {
        ShowWindowsContextMenu(window, button, action, mods);
    }
#endif

    return;
}


void GLFW_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    return;
}


