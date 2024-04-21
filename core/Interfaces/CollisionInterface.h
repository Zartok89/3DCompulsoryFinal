#pragma once
#include <iostream>
#include <unordered_map>
#include <utility>
#include "mathematics/BarycentricC.h"
#include "physics/AABBcollision.h"

class ICollision
{
public:
	virtual ~ICollision() = default;

	virtual AABBcollision GetAABB() const = 0;

	// Creating a empty function pointer
	using FunctionPtr = void (*)();

	// Index 1 = AABB, Index 2 = Barycentric Coordinates Collision
	virtual void (*ChooseCollisionType(int index, Mesh* mesh))
	{
		switch (index)
		{
		case 1:
			//glm::vec3* Vec3Ptr;
			//AABB(glm::vec3(1.f), glm::vec3(4.f), Vec3Ptr);
			break;
		case 2:
			BarycentricCollision(mesh);
			break;
		default:
			return nullptr;
		}
		return nullptr;
	}

	void BarycentricCollision(class Mesh* staticMesh) const
	{
		std::cout << "Hello! I am function pointer\n";
		BarycentricC::getBarycentricCoordinatesActor(staticMesh);
	}

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