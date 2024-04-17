#pragma once
#include "actors/Actor.h"

// Change this variable in the shader too
#define MAX_POINT_LIGHTS 64

class PointLight : public Actor
{
public:
	~PointLight() = default; //Deconstructor

	PointLight(const std::string& name) :Actor(name) {};
		
	virtual glm::vec3 GetPosition() const
	{
		return GetGlobalPosition();
	}

	/*
	* Member variables
	*/
	glm::vec3 mAmbient{0.5f, 0.5f, 0.5f};
	glm::vec3 mColor{1.f, 1.f, 1.f};
	float mConstant = 1.f;
	float mLinear = 0.09f;
	float mQuadratic = 0.032f;
};