#include "PawnActor.h"

PawnActor::PawnActor(const std::string& name, Mesh* mesh) : Actor(name)
{
	mMesh = mesh;
}

void PawnActor::Draw(const Shader* shader) const
{
	if (!mMesh) return;
	mMesh->DrawMesh(shader);
}

void* PawnActor::ChooseCollisionType(int index)
{
	return ICollision::ChooseCollisionType(index);
}
