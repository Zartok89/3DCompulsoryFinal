#include "actors/MeshActor.h"

void MeshActor::Draw(const Shader* shader) const
{
	if (!mMesh) return;
	mMesh->DrawMesh(shader);
}

void* MeshActor::ChooseCollisionType(int index)
{
	return ICollision::ChooseCollisionType(index);
}
