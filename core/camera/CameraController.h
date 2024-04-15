#pragma once

#include "interfaces/ControllerInterface.h"
#include <map>

class CameraController : public IController
{
public:
    CameraController(class Camera* camera) : mCamera(camera) {}

    /*
     * Overrides from the Controller Interface to handle input and movement logic for the camera
     */
    virtual void HandleMouseMovement(class Window* window, double xpos, double ypos) override;
    virtual void HandleMouseScroll(class Window* window, double xoffset, double yoffset) override;
    virtual void HandleMouseButton(class Window* window, int button, int action, int mods) override;
    virtual void HandleKeyboard(class Window* window, int key, int scancode, int action, int mods) override;

private:
    void UpdateCameraAcceleration(); // WASD Movement with the keys

    float mLastX = 0.0, mLastY = 0.0;
    bool mRightMousePressed = false;
    float mMouseSensitivity = 0.1f;

    std::map<int, bool> mKeyStates; // Binding keys to their state to see if its pressed or not
    class Camera* mCamera;
};