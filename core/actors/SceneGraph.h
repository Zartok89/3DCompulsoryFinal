#pragma once

#include "actors/Actor.h"

class SceneGraph : public Actor
{
public:
	SceneGraph(const std::string& name) : Actor(name) { }; // Scene graph constructor
};