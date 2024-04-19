#pragma once
#include <map>
#include "camera/Camera.h"
#include "Interfaces/ControllerInterface.h"

class ActorController : public IController
{
public:
	ActorController(class Actor* actor, bool bIsCameraAttachedToPlayer, Camera* sceneCamera); // Constructor
	void MovementNoCameraFollow(float dt);
	void MovementWithCameraFollow(float dt);

	/*
	* Virtual Functions from the controllerinterface
	*/
	virtual void UpdateCurrentController(float dt) override;
    virtual void HandleMouseMovement(class Window* window, double xpos, double ypos) override;
    virtual void HandleMouseScroll(class Window* window, double xoffset, double yoffset) override;
    virtual void HandleMouseButton(class Window* window, int button, int action, int mods) override;
    virtual void HandleKeyboard(class Window* window, int key, int scancode, int action, int mods) override;

	/*
	* Member variables
	*/
	float mMovementSpeed;
	Actor* mActor;
	Camera* mCamera;
	std::map<int, bool> mKeyStates;
	bool mIsAttachedToPlayer = false;
};