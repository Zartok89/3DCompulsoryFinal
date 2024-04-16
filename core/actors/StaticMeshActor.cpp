#include "StaticMeshActor.h"

StaticMeshActor::StaticMeshActor(const std::string& name, Mesh* mesh) : Actor(name)
{
	mMesh = mesh;
}

void StaticMeshActor::Draw(const Shader* shader) const
{
	if (!mMesh) return;
	mMesh->DrawMesh(shader);
}

void* StaticMeshActor::ChooseCollisionType(int index)
{
	return ICollision::ChooseCollisionType(index);
}
