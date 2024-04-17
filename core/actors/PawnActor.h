#pragma once
#include "actors/Actor.h"
#include "Interfaces/RenderInterface.h"
#include "Interfaces/CollisionInterface.h"
#include "graphics/Mesh.h"

class PawnActor : public Actor, public IRender, public ICollision
{
public:
    PawnActor(const std::string& name, Mesh* mesh); // Constructor

    /*
     * Virtual Functions
     */
    void Draw(const Shader* shader = nullptr) const override;
	void* ChooseCollisionType(int index) override; // Index 1 = AABB, Index 2 = Barycentric Coordinates Collision

    /*
     * Member variables
     */
    Mesh* mMesh{ nullptr };
};