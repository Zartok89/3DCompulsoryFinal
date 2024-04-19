#include "ActorController.h"
#include "actors/Actor.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"

ActorController::ActorController(Actor* actor, bool bIsCameraAttachedToPlayer, Camera* sceneCamera)
{
	mActor = actor;
    mMovementSpeed = 2.f;
    mIsAttachedToPlayer = bIsCameraAttachedToPlayer;
	mCamera = sceneCamera;
}

void ActorController::MovementNoCameraFollow(float dt)
{
	if (mKeyStates[GLFW_KEY_D])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() + glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
	}
	if (mKeyStates[GLFW_KEY_A])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() - glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
	}
	if (mKeyStates[GLFW_KEY_W])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() - glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
	}
	if (mKeyStates[GLFW_KEY_S])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() + glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
	}
}

void ActorController::MovementWithCameraFollow(float dt)
{
	auto acceleration = mCamera->GetAcceleration();
    float movementSpeed = mCamera->GetAccelerationSpeed();
	mCamera->SetLocalPosition(mActor->GetGlobalPosition() - glm::vec3(0.f, -2.f, -8.f));
	mCamera->SetLocalRotation(glm::angleAxis((glm::radians(0.f)), glm::vec3(0.f, 1.f, 0.f)));

    acceleration = glm::vec3(0.f);

	if (mKeyStates[GLFW_KEY_D])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() + glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
		acceleration.x += movementSpeed;
		mCamera->SetAcceleration(acceleration);
		
	}
	if (mKeyStates[GLFW_KEY_A])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() - glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
		acceleration.x -= movementSpeed;
		mCamera->SetAcceleration(acceleration);
	}
	if (mKeyStates[GLFW_KEY_W])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() - glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
		acceleration.z += movementSpeed;
		mCamera->SetAcceleration(acceleration);
	}
	if (mKeyStates[GLFW_KEY_S])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() + glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
		acceleration.z -= movementSpeed;
		mCamera->SetAcceleration(acceleration);
	}
}

void ActorController::UpdateCurrentController(float dt)
{
	if (mIsAttachedToPlayer)
	{
		MovementWithCameraFollow(dt);
	}
	else
	{
		MovementNoCameraFollow(dt);
	}
}

void ActorController::HandleMouseMovement(Window* window, double xpos, double ypos)
{
}

void ActorController::HandleMouseScroll(Window* window, double xoffset, double yoffset)
{
}

void ActorController::HandleMouseButton(Window* window, int button, int action, int mods)
{
}

void ActorController::HandleKeyboard(Window* window, int key, int scancode, int action, int mods)
{
	if (!mActor) return;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        mKeyStates[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        mKeyStates[key] = false;
    }
}
