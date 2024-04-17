#pragma once
#include <iostream>
#include <unordered_map>
#include <utility>
#include "mathematics/BarycentricC.h"
#include "physics/AABBcollision.h"

class BarycentricC;
class ICollision
{
public:
	// Creating a empty function pointer
	using FunctionPtr = void (*)();

	// Index 1 = AABB, Index 2 = Barycentric Coordinates Collision
	virtual void (*ChooseCollisionType(int index))
	{
		switch (index)
		{
		case 1:
			glm::vec3* Vec3Ptr;
			AABB(glm::vec3(1.f), glm::vec3(4.f), Vec3Ptr);
			break;
		case 2:
			BarycentricCollision(mStaticMeshPointer);
			break;
		default:
			return nullptr;
		}
		return nullptr;
	}

	void BarycentricCollision(class PawnActor* staticMesh) const
	{
		std::cout << "Hello! I am function pointer\n";
		mBarycentricCPtr->getBarycentricCoordinatesActor(staticMesh);
	}

	void AABB(const glm::vec3& center, const glm::vec3& extent, glm::vec3* mtv)
	{
		AABBcollision(center, extent);
		AABBPtr->Intersect(AABBcollision(center, extent), mtv);
	}

	/*
	 * Pointers to other classes
	 */
	BarycentricC* mBarycentricCPtr;
	AABBcollision* AABBPtr;
	PawnActor* mStaticMeshPointer;

};