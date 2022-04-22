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

    float objectMovementSpeed = 0.1f;
    float lightMovementSpeed = 10.0f;

    if ( cGFLWKeyboardModifiers::areAllModsUp(pWindow) )
    {
        if (!::g_pGameEngine->g_pGameplayManager->GetAiming()) {
            // Use "fly" camera (keyboard for movement, mouse for aim)
            //if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
            //{
            //    ::g_pFlyCamera->MoveForward_Z(+cameraMoveSpeed);
            //}
            //if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
            //{
            //    ::g_pFlyCamera->MoveForward_Z(-cameraMoveSpeed);
            //}
        }
        //if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS )	// "left"
        //{
        //    ::g_pFlyCamera->MoveLeftRight_X(-cameraMoveSpeed);
        //}
        //if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS )	// "right"
        //{
        //    ::g_pFlyCamera->MoveLeftRight_X(+cameraMoveSpeed);
        //}
        //if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS )	// "up"
        //{
        //    ::g_pFlyCamera->MoveUpDown_Y(-cameraMoveSpeed);
        //}
        //if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS )	// "down"
        //{
        //    ::g_pFlyCamera->MoveUpDown_Y(+cameraMoveSpeed);
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


    }//if ( cGFLWKeyboardModifiers::...

    return;
}

bool checkMovementKeysDown() {
    return (glfwGetKey(::g_pWindow, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(::g_pWindow, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(::g_pWindow, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(::g_pWindow, GLFW_KEY_D) == GLFW_PRESS);
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
    if ( (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        && ::g_MouseIsInsideWindow )
    {
        if (!::g_pGameEngine->g_pGameplayManager->GetAiming()) {
            ::g_pGameEngine->entityManager.GetPlayer()->SetNeutralOrientation();
        }
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // Mouse button is down so turn the camera
        ::g_pFlyCamera->Yaw_LeftRight(::g_pFlyCamera->getDeltaMouseX() * MOUSE_SENSITIVITY, deltaTime);

        ::g_pFlyCamera->Pitch_UpDown(-::g_pFlyCamera->getDeltaMouseY() * MOUSE_SENSITIVITY, deltaTime);
        ::g_pGameEngine->g_pGameplayManager->SetAiming(true);
        //TODO: Restore player rotation
        ::g_pGameEngine->entityManager.GetPlayer()->rotation = ::g_pFlyCamera->getQOrientation();
    }
    else if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) && ::g_pGameEngine->g_pGameplayManager->GetAiming()) {
        ::g_pGameEngine->g_pGameplayManager->SetAiming(false);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        ::g_pGameEngine->entityManager.GetPlayer()->ResetToNeutralOrientation();
    }
    // Mouse button is down so turn the camera
    //if (::g_MouseIsInsideWindow) {

    //    ::g_pFlyCamera->Yaw_LeftRight(-::g_pFlyCamera->getDeltaMouseX() * MOUSE_SENSITIVITY, deltaTime);

    //    ::g_pFlyCamera->Pitch_UpDown(-::g_pFlyCamera->getDeltaMouseY() * MOUSE_SENSITIVITY, deltaTime);
    //}

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
        ::g_MouseIsInsideWindow = true;
    }
    else
    {
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

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && ::g_pGameEngine->g_pGameplayManager->GetAiming()) {
        ::g_pGameEngine->g_pGameplayManager->Fire();

    }
#ifdef YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG
    // Right button is pop-up
    if ( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS )
    {
        ShowWindowsContextMenu(window, button, action, mods);
    }
#endif

    // Send a notification to the character that a key was pressed.
    // Could use pub/sub model if we were broadcasting to more entities, but for now, we only have one that cares.
    if (action == GLFW_PRESS) {
        GameEvent_MousePress* g_event = new GameEvent_MousePress(button);
        ::g_pGameEngine->entityManager.GetPlayer()->Notify(GameEventType::MOUSE_PRESS, g_event);
        delete g_event;
    }
    else if (action == GLFW_RELEASE) {
        GameEvent_MouseRelease* g_event = new GameEvent_MouseRelease(button);
        ::g_pGameEngine->entityManager.GetPlayer()->Notify(GameEventType::MOUSE_RELEASE, g_event);
        delete g_event;
    }
    return;
}


void GLFW_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    return;
}


