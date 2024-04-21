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

//void* MeshActor::ChooseCollisionType(int index,  std::string* fbx_Path)
//{
//	return ICollision::ChooseCollisionType(index,fbx_Path); 
//}
