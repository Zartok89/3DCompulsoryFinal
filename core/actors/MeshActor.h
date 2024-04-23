#pragma once
#include "Actor.h"
#include "Interfaces/RenderInterface.h"
#include "Interfaces/CollisionInterface.h"
#include "graphics/Mesh.h"

class MeshActor : public Actor, public IRender, public ICollision
{
public:

	MeshActor(const std::string& name) : Actor(name) {}
    MeshActor(const std::string& name, Mesh* mesh) : Actor(name), mMesh(mesh){}

    /*
     * Virtual Functions
     */
    void Draw(const Shader* shader = nullptr) const override;

	// Index 1 = AABB, Index 2 = Barycentric Coordinates Collision
	//void* ChooseCollisionType(int index, Mesh*) override;

    /*
     * Member variables
     */
    Mesh* mMesh{ nullptr };
};