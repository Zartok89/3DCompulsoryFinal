#pragma once

#include <glm/glm.hpp>
#include "actors/Actor.h"

class DirectionalLight : public Actor
{
public:
	~DirectionalLight() = default; //Deconstructor

	DirectionalLight(const std::string& name) : Actor(name) {}

	glm::vec3 GetDirection() const { return GetLocalRotation() * glm::vec3(0.f, 0.f, -1.f); }

	inline void SetLightRotation(const float& rotationDegrees, int axisX, int axisY, int axisZ)
	{
		axisX = std::clamp(axisX, 0, 1);
		axisY = std::clamp(axisY, 0, 1);
		axisZ = std::clamp(axisZ, 0, 1);
		mTransform.SetRotation(glm::angleAxis((glm::radians(rotationDegrees)), glm::vec3(axisX, axisY, axisZ)));
	}

	/*
	* Member variables
	*/
	glm::vec3 mAmbient{ 0.5f, 0.5f, 0.5f };
	glm::vec3 mColor{ 1.f, 1.f, 1.f };
};