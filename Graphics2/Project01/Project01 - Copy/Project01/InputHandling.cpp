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
    float cameraMoveSpeed = ::g_pActiveCamera->movementSpeed;

    float objectMovementSpeed = 0.1f;
    float lightMovementSpeed = 10.0f;

    if ( cGFLWKeyboardModifiers::areAllModsUp(pWindow) )
    {
        // Use "fly" camera (keyboard for movement, mouse for aim)
        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS )
        {
            ::g_pActiveCamera->MoveForward_Z(+cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS )	// "backwards"
        {
            ::g_pActiveCamera->MoveForward_Z(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS )	// "left"
        {
            ::g_pActiveCamera->MoveLeftRight_X(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS )	// "right"
        {
            ::g_pActiveCamera->MoveLeftRight_X(+cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS )	// "up"
        {
            ::g_pActiveCamera->MoveUpDown_Y(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS )	// "down"
        {
            ::g_pActiveCamera->MoveUpDown_Y(+cameraMoveSpeed);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_L) == GLFW_PRESS)	// "down"
        {
            ::g_pActiveCamera = ::g_pFlyCamera;
        }
        if (glfwGetKey(pWindow, GLFW_KEY_T) == GLFW_PRESS)	// "down"
        {
            ::g_pActiveCamera = ::g_pTVCamera;
        }


        if (glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS)	// "down"
        {
            ::g_pTheLights->TurnOnLight(0);
        }
        else if (::g_pTheLights->theLights[0].param2.x == 1.0f && glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_RELEASE)	// If light is on and key is released, turn off.
        {
            ::g_pTheLights->TurnOffLight(0);
        }

        if (glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_PRESS)	// "down"
        {
            ::g_pTheLights->TurnOnLight(1);
        }
        else if (::g_pTheLights->theLights[1].param2.x == 1.0f && glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_RELEASE)	// If light is on and key is released, turn off.
        {
            ::g_pTheLights->TurnOffLight(1);
        }

        std::stringstream strTitle;
        // std::cout << 
        glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
        strTitle << "Camera: "
            << cameraEye.x << ", "
            << cameraEye.y << ", "
            << cameraEye.z; //<< std::endl;

        #if defined _DEBUG
        ::g_TitleText = strTitle.str();
        #endif

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
        #if defined _DEBUG
        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.x -= lightMovementSpeed; } // Go left
        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.x += lightMovementSpeed; } // Go right
        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.z += lightMovementSpeed; }// Go forward 
        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.z -= lightMovementSpeed; }// Go backwards
        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.y -= lightMovementSpeed; }// Go "Down"
        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS ) { ::g_pTheLights->theLights[::g_selectedLight].position.y += lightMovementSpeed; }// Go "Up"

        // constant attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS )
        {
            ::g_pTheLights->theLights[::g_selectedLight].atten.x *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::g_pTheLights->theLights[::g_selectedLight].atten.x <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::g_pTheLights->theLights[::g_selectedLight].atten.x = 0.001f;
            }
            ::g_pTheLights->theLights[::g_selectedLight].atten.x *= 1.01f; // +1% more
        }
        // linear attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_3) == GLFW_PRESS )
        {
            ::g_pTheLights->theLights[::g_selectedLight].atten.y *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_4) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::g_pTheLights->theLights[::g_selectedLight].atten.y <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::g_pTheLights->theLights[::g_selectedLight].atten.y = 0.001f;
            }
            ::g_pTheLights->theLights[::g_selectedLight].atten.y *= 1.01f; // +1% more
        }
        // quadratic attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_5) == GLFW_PRESS )
        {
            ::g_pTheLights->theLights[::g_selectedLight].atten.z *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_6) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::g_pTheLights->theLights[::g_selectedLight].atten.z <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::g_pTheLights->theLights[::g_selectedLight].atten.z = 0.001f;
            }
            ::g_pTheLights->theLights[::g_selectedLight].atten.z *= 1.01f; // +1% more
        }

        if ( glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS ) { ::g_pTheLights->theLights[0].param1.y -= 0.5f; }   // Inner
        if ( glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS ) { ::g_pTheLights->theLights[0].param1.y += 0.5f; }   // Inner
        if ( glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS ) { ::g_pTheLights->theLights[0].param1.z -= 0.5f; }   // Outer
        if ( glfwGetKey(pWindow, GLFW_KEY_P) == GLFW_PRESS ) { ::g_pTheLights->theLights[0].param1.z += 0.5f; }   // Outer


        std::stringstream strTitle;
        // std::cout << 
        strTitle << "Light # " << ::g_selectedLight << " positionXYZ : "
            << ::g_pTheLights->theLights[::g_selectedLight].position.x << ", "
            << ::g_pTheLights->theLights[::g_selectedLight].position.y << ", "
            << ::g_pTheLights->theLights[::g_selectedLight].position.z << "  "
            << "attenuation (C, L, Q): "
            << ::g_pTheLights->theLights[::g_selectedLight].atten.x << ", "        // Const
            << ::g_pTheLights->theLights[::g_selectedLight].atten.y << ", "        // Linear
            << ::g_pTheLights->theLights[::g_selectedLight].atten.z << "  "        // Quadratic
            << (::g_pTheLights->theLights[::g_selectedLight].param2.x > 0.0f ? " is on" : " is off");
        //<< std::endl;
        ::g_TitleText = strTitle.str();
    #endif

    }//if ( cGFLWKeyboardModifiers::...

    return;
}


// We call these every frame
void handleAsyncMouse(GLFWwindow* window, double deltaTime)
{

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    ::g_pActiveCamera->setMouseXY(x, y);

    const float MOUSE_SENSITIVITY = 2.0f;


    // Mouse left (primary?) button pressed? 
    // AND the mouse is inside the window...
    if ( (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        && ::g_MouseIsInsideWindow )
    {
        // Mouse button is down so turn the camera
        ::g_pActiveCamera->Yaw_LeftRight( ::g_pActiveCamera->getDeltaMouseX() * MOUSE_SENSITIVITY, deltaTime );

        ::g_pActiveCamera->Pitch_UpDown( -::g_pActiveCamera->getDeltaMouseY() * MOUSE_SENSITIVITY, deltaTime );

    }

    // Adjust the mouse speed
    if ( ::g_MouseIsInsideWindow )
    {
        const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

        // Adjust the movement speed based on the wheel position
        ::g_pActiveCamera->movementSpeed -= ( ::g_pActiveCamera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY );

        // Clear the mouse wheel delta (or it will increase constantly)
        ::g_pActiveCamera->clearMouseWheelValue();


        if ( ::g_pActiveCamera->movementSpeed <= 0.0f )
        {
            ::g_pActiveCamera->movementSpeed = 0.0f;
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

    ::g_pActiveCamera->setMouseWheelDelta(yoffset * mouseScrollWheelSensitivity);

    return;
}

void GLFW_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

#if defined _DEBUG
#ifdef Windows_Context_Menu_Usage_Custom

    // Right button is pop-up
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        ShowWindowsContextMenu(window, button, action, mods);
    }
#endif
#endif

    return;
}


void GLFW_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    return;
}


