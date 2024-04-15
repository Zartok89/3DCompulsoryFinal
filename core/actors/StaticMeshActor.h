#pragma once
#include "actors/Actor.h"
#include "Interfaces/RenderInterface.h"
#include "graphics/Mesh.h"

class StaticMeshActor : public Actor, public IRender
{
public:
    StaticMeshActor(const std::string& name, Mesh* mesh);
    virtual void Draw(const class Shader* shader = nullptr) const;
    Mesh* mMesh{ nullptr };
};