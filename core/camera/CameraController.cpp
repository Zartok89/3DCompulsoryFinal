#include "CameraController.h"
#include "utility/Logger.h"
#include "Camera.h"
#include "program/Application.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

void CameraController::HandleMouseMovement(Window* window, double xpos, double ypos)
{
    if (!mRightMousePressed) return;

    float xoffset = mLastX - xpos;
    float yoffset = mLastY - ypos;

    mLastX = xpos;
    mLastY = ypos;

    xoffset *= mMouseSensitivity;
    yoffset *= mMouseSensitivity;

    float yawRad = glm::radians(xoffset);
    float pitchRad = glm::radians(yoffset);

    glm::quat currentOrientation = mCamera->GetLocalRotation();
    glm::quat yawRotation = glm::angleAxis(yawRad, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat pitchRotation = glm::angleAxis(pitchRad, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat newOrientation = yawRotation * currentOrientation * pitchRotation;
    newOrientation = glm::normalize(newOrientation);
    mCamera->SetLocalRotation(newOrientation);
}

void CameraController::HandleMouseButton(Window* window, int button, int action, int mods)
{ 
    if (button == GLFW_MOUSE_BUTTON_RIGHT) 
    {
        if (action == GLFW_PRESS) 
        {
            mRightMousePressed = true;
            glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            double xpos, ypos;
            glfwGetCursorPos(window->GetGLFWWindow(), &xpos, &ypos);
            mLastX = static_cast<float>(xpos);
            mLastY = static_cast<float>(ypos);
        }
        else if (action == GLFW_RELEASE) 
        {
            mRightMousePressed = false;
            glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void CameraController::UpdateCurrentController(float dt)
{
    auto acceleration = mCamera->GetAcceleration();
    float movementSpeed = mCamera->GetAccelerationSpeed();
    acceleration = glm::vec3(0.f);

    if (mKeyStates[GLFW_KEY_W]) acceleration.z += movementSpeed;
    if (mKeyStates[GLFW_KEY_S]) acceleration.z -= movementSpeed;
    if (mKeyStates[GLFW_KEY_A]) acceleration.x -= movementSpeed;
    if (mKeyStates[GLFW_KEY_D]) acceleration.x += movementSpeed;
    if (mKeyStates[GLFW_KEY_E]) acceleration.y += movementSpeed;
    if (mKeyStates[GLFW_KEY_Q]) acceleration.y -= movementSpeed;
 

    if (mKeyStates[GLFW_KEY_LEFT_SHIFT]) mCamera->SetAccelerationSpeed(200.f);
    else if (!mKeyStates[GLFW_KEY_LEFT_SHIFT]) mCamera->SetAccelerationSpeed(50.f);

    mCamera->SetAcceleration(acceleration);
}

void CameraController::HandleKeyboard(Window* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT) 
    {
        mKeyStates[key] = true;
    }
    else if (action == GLFW_RELEASE) 
    {
        mKeyStates[key] = false;
    }

    UpdateCurrentController(0);
    //UpdateCameraAcceleration();
}

void CameraController::HandleMouseScroll(Window* window, double xoffset, double yoffset){}