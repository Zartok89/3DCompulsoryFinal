#pragma once
#include <iostream>
#include <unordered_map>
#include <utility>
#include "mathematics/BarycentricC.h"

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
			return 0/*AABB()*/;
			break;
		case 2:
			BarycentricCollision(mStaticMeshPointer);
			break;
		default:
			return nullptr;
		}
		return nullptr;
	}

	void BarycentricCollision(class StaticMeshActor* staticMesh) const
	{
		std::cout << "Hello! I am function pointer\n";
		mBarycentricCPtr->getBarycentricCoordinatesActor(staticMesh);
	}

	/*
	 * Member Variables
	 */
	BarycentricC* mBarycentricCPtr;
	StaticMeshActor* mStaticMeshPointer;

};