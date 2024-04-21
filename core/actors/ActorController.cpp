#include "ActorController.h"

#include "imgui.h"
#include "actors/Actor.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"

ActorController::ActorController(Actor* actor, bool bIsCameraAttachedToPlayer, Camera* sceneCamera)
{
	mActor = actor;
    mMovementSpeed = 5.f;
    mIsAttachedToPlayer = bIsCameraAttachedToPlayer;
	mCamera = sceneCamera;
}

void ActorController::MovementNoCameraFollow(float dt)
{
	if (mKeyStates[GLFW_KEY_D])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() + glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt); 
		mActor->SetLocalRotation(glm::quat(glm::radians(90.f), 0, 1, 0));
	}
	if (mKeyStates[GLFW_KEY_A])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() - glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
		mActor->SetLocalRotation(glm::quat(glm::radians(-90.f), 0, 1, 0));
	}
	if (mKeyStates[GLFW_KEY_W])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() - glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
		mActor->SetLocalRotation(glm::quat(0, 0, 1, 0));

	}
	if (mKeyStates[GLFW_KEY_S])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() + glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
		mActor->SetLocalRotation(glm::quat(0, 0, 0, 0)); 
	}
	if (mKeyStates[GLFW_KEY_LEFT_SHIFT])
	{
		mMovementSpeed = 10.f;
	}
	else if (!mKeyStates[GLFW_KEY_LEFT_SHIFT])
	{
		mMovementSpeed = 5.f;
	}
}

void ActorController::MovementWithCameraFollow(float dt)
{
	RenderActorGUI();
	auto acceleration = mCamera->GetAcceleration();
    float movementSpeed = mCamera->GetAccelerationSpeed();
	mCamera->SetLocalPosition(mActor->GetGlobalPosition() - glm::vec3(mCameraPositionX, mCameraPositionY, mCameraPositionZ));
	mCamera->SetLocalRotation(glm::angleAxis((glm::radians(0.f)), glm::vec3(0.f, 1.f, 0.f)));
	mCamera->SetLocalRotation(glm::angleAxis((glm::radians(mCameraAngle)), glm::vec3(1.f, 0.f, 0.f)));

    acceleration = glm::vec3(0.f);

	if (mKeyStates[GLFW_KEY_D])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() + glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
		acceleration.x += movementSpeed;
		mCamera->SetAcceleration(acceleration);
		mActor->SetLocalRotation(glm::quat(glm::radians(90.f), 0, 1, 0)); 
	}
	if (mKeyStates[GLFW_KEY_A])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() - glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
		acceleration.x -= movementSpeed;
		mCamera->SetAcceleration(acceleration);
		mActor->SetLocalRotation(glm::quat(glm::radians(-90.f), 0, 1, 0));
	}
	if (mKeyStates[GLFW_KEY_W])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() - glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
		acceleration.z += movementSpeed;
		mCamera->SetAcceleration(acceleration);
		mActor->SetLocalRotation(glm::quat(0, 0, 1, 0));
	}
	if (mKeyStates[GLFW_KEY_S])
	{
		mActor->SetLocalPosition(mActor->GetGlobalPosition() + glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
		acceleration.z -= movementSpeed;
		mCamera->SetAcceleration(acceleration);
		mActor->SetLocalRotation(glm::quat(0, 0, 0, 0));
	}
	if (mKeyStates[GLFW_KEY_E])
	{
		mCamera->SetLocalRotation(glm::angleAxis((glm::radians(90.f)), glm::vec3(1.f, 0.f, 0.f)));
	}
	if (mKeyStates[GLFW_KEY_LEFT_SHIFT])
	{
		mMovementSpeed = 10.f;  
	}
	else if (!mKeyStates[GLFW_KEY_LEFT_SHIFT])
	{
		mMovementSpeed = 5.f;
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

void ActorController::RenderActorGUI()
{
	ImGui::Begin("Player Camera Values:");
	ImGui::SliderFloat("Player y position: ", &mCameraPositionY, 0.f*-1.f, 10.f*-1.f);
	ImGui::SliderFloat("Player z position: ", &mCameraPositionZ, 5.f*-1.f, 20.f*-1.f);


	ImGui::End();
}