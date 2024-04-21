#pragma once
#include <iostream>
#include <unordered_map>
#include <utility>
#include "mathematics/BarycentricC.h"
#include "physics/AABBcollision.h"
#include "string"

class ICollision
{
public:
	virtual ~ICollision() = default;

	virtual AABBcollision GetAABB() const = 0;

	// Creating a empty function pointer
	using FunctionPtr = void (*)();

	// Index 1 = AABB, Index 2 = Barycentric Coordinates Collision
	//virtual void (*ChooseCollisionType(int index,std::string* fbx_Path))
	//{
	//	switch (index)
	//	{
	//	case 1:
	//		//glm::vec3* Vec3Ptr;
	//		//AABB(glm::vec3(1.f), glm::vec3(4.f), Vec3Ptr);
	//		break;
	//	case 2:
	//		BarycentricCollision(fbx_Path);
	//		break;
	//	default:
	//		return nullptr;
	//	}
	//	return nullptr;
	//}

	//void BarycentricCollision(std::string* fbx_Path) const
	//{
	//	std::cout << "Function in use\n";
	//	BarycentricC::getBarycentricCoordinatesActor(fbx_Path);
	//	std::cout << "Function completed\n";
	//}

	void AABB(const glm::vec3& center, const glm::vec3& extent, glm::vec3* mtv)
	{
		//AABBcollision(center, extent);
		//AABBPtr->Intersect(AABBcollision(center, extent), mtv);
	}

	/*
	 * Pointers to other classes
	 */
	//BarycentricC* mBarycentricCPtr;
	//AABBcollision* AABBPtr;
	//PawnActor* mStaticMeshPointer;

};