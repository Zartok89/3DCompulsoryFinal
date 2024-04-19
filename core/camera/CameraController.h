#pragma once

#include "interfaces/ControllerInterface.h"
#include <map>

class CameraController : public IController
{
public:
    CameraController(class Camera* camera) : mCamera(camera) {}

    //void UpdateCameraAcceleration(); // WASD Movement with the keys

    /*
     * Overrides from the Controller Interface to handle input and movement logic for the camera
     */
    void UpdateCurrentController(float dt) override;
    void HandleMouseMovement(Window* window, double xpos, double ypos) override;
    void HandleMouseScroll(Window* window, double xoffset, double yoffset) override;
    void HandleMouseButton(Window* window, int button, int action, int mods) override;
    void HandleKeyboard(Window* window, int key, int scancode, int action, int mods) override;

    /*
     * Member variables
     */
    float mLastX = 0.0, mLastY = 0.0;
    bool mRightMousePressed = false;
    float mMouseSensitivity = 0.1f;
    std::map<int, bool> mKeyStates; // Binding keys to their state to see if its pressed or not
    Camera* mCamera;
};