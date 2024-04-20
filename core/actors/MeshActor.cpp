#include "actors/MeshActor.h"

void MeshActor::Draw(const Shader* shader) const
{
	if (!mMesh) return;
	mMesh->DrawMesh(shader);
}

AABBcollision MeshActor::GetAABB() const
{
	return AABBcollision(GetGlobalPosition(), GetGlobalScale() * 0.5f);
}

void* MeshActor::ChooseCollisionType(int index)
{
	return ICollision::ChooseCollisionType(index);
}
