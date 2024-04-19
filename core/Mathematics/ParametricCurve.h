#pragma once
#include <iostream>
#include <string>
#include <actors/PawnActor.h>
#include <actors/Actor.h>
#include <actors/StaticMeshActor.h>
#include <graphics/Vertex.h>
#include <glm/glm.hpp>

class ParametricCurve : public Actor
{

	//Function for calculating third degree polynomial. Takes X or Z
	glm::mat4x4 CreateRandomPointsMatrix(const double& randomNum1, const double& randomNum2, const double& randomNum3, const double& randomNum4)
	{
		//Insert random numbers for columns except y
		return glm::mat4x2{ randomNum1,1,
			                randomNum2,1,  
						    randomNum3,1,
			                randomNum4,1 
		                 };
	}
	glm::mat4x4  Fx(const glm::mat4x4& interpolationPoints) 
	{
		glm::mat4x4 getMatrix = {
		interpolationPoints[0],
		interpolationPoints[1],
		interpolationPoints[2],
		interpolationPoints[3]
		}; 

		glm::vec4 F1 = glm::vec4{ pow(getMatrix[0].x,3.f), pow(getMatrix[0].x,2.f), getMatrix[0].x, 1.f };
		glm::vec4 F2 = glm::vec4{ pow(getMatrix[1].x,3.f), pow(getMatrix[1].x,2.f), getMatrix[1].x, 1.f };
		glm::vec4 F3 = glm::vec4{ pow(getMatrix[2].x,3.f), pow(getMatrix[2].x,2.f), getMatrix[2].x, 1.f };
		glm::vec4 F4 = glm::vec4{ pow(getMatrix[3].x,3.f), pow(getMatrix[3].x,2.f), getMatrix[3].x, 1.f };

		glm::mat4x4 finalMatrix
		{
			F1,
			F2,
			F3,
			F4
		};

		return finalMatrix; 
	}

	void P(const glm::mat4x4& getFinalMatrix, const double& n)
	{
	     auto calcedFinalMatrix = glm::inverse(getFinalMatrix); 
		 //std::cout << getFinalMatrix << std::endl; 
	/*	 glm::vec4 A_1b{ calcedFinalMatrix[0],calcedFinalMatrix[1],calcedFinalMatrix[3], calcedFinalMatrix[3] };
		 auto P = A_1b.x * (pow(n, 3)) + A_1b.y * pow(n, 2) + A_1b.z * n;*/
	}

public:

	void CreateInterpolationCurve4Points(double startVal, double endingVal, double resolution, StaticMeshActor* staticMesh) 
	{
		///Creating random numbers
		srand((unsigned) time(NULL));

		auto random1 = staticMesh->GetLocalPosition().x + (rand() % + 10);
		auto random2 = staticMesh->GetLocalPosition().x + (rand() % + 11);
		auto random3 = staticMesh->GetLocalPosition().x + (rand() % + 12);
		auto random4 = staticMesh->GetLocalPosition().x + (rand() % + 13);
		

		auto a = CreateRandomPointsMatrix(random1,random2,random3,random4);

		// Output the coefficients
		std::cout << "Create Matrix " << a << "\n";
		std::cout << "\n";


		auto b = Fx(a);
		std::cout << "Final Matrix" << b << "\n";
		


		//Matrification
		for (double i = startVal; startVal <= endingVal; i+= resolution)
		{
			P(b, i);
		}
	}



};
