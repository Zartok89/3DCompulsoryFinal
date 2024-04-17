#pragma once

#include <glm/glm.hpp>
#include "actors/Actor.h"

class DirectionalLight : public Actor
{
public:
	~DirectionalLight() = default; //Deconstructor

	DirectionalLight(const std::string& name) : Actor(name) {}

	glm::vec3 GetDirection() const { return GetLocalRotation() * glm::vec3(0.f, 0.f, -1.f); }

	/*
	* Member variables
	*/
	glm::vec3 mAmbient{0.5f, 0.5f, 0.5f};
	glm::vec3 mColor{1.f, 1.f, 1.f};
};