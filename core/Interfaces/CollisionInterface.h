#pragma once
#include <functional>

class ICollision
{
public:
	// Index 1 = AABB, Index 2 = Barycentric Coordinates Collision
	inline virtual void (*ChooseCollisionType(int index))
	{
		switch (index)
		{
		case 1:
			return AABB();
			break;
		case 2:
			return BarycentricCollision();
			break;
		default:
			return 0;
		}
	};
};