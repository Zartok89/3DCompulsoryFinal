#pragma once
#include "actors/Actor.h"
#include "Interfaces/RenderInterface.h"
#include "Interfaces/CollisionInterface.h"
#include "graphics/Mesh.h"

class StaticMeshActor : public Actor, public IRender, public ICollision
{
public:
    StaticMeshActor(const std::string& name, Mesh* mesh); // Constructor

    /*
     * Virtual Functions
     */
    void Draw(const Shader* shader = nullptr) const override;
    void* ChooseCollisionType(int index) override;

    /*
     * Member variables
     */
    Mesh* mMesh{ nullptr };
};