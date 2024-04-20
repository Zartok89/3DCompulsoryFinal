#pragma once
#include <iostream>
#include <string>
#include <actors/Actor.h>
#include <actors/MeshActor.h>
#include <graphics/Vertex.h>
#include <glm/glm.hpp>

class ParametricCurve : public Actor
{

	//Creates a matrix with points to interpolate over
	glm::mat4x2 PointMatrix(const double& randomX1, const double& randomX2, const double& randomX3, const double& randomX4)
	{
		//Insert random numbers for columns except y
		return glm::mat4x2{ randomX1, randomX2,
			                randomX3, randomX4,
			                randomX4, randomX1,
			                randomX2, randomX1
		                  };
	}

	//Matrifying the POintMatrix
	glm::mat4x4  Y(const glm::mat4x2& pointVector)
	{
		
		glm::vec4 t1 = glm::vec4{ pow(pointVector[0].x,3.f), pow(pointVector[0].x,2.f), pointVector[0].x, 1.f };
		glm::vec4 t2 = glm::vec4{ pow(pointVector[1].x,3.f), pow(pointVector[1].x,2.f), pointVector[1].x, 1.f };
		glm::vec4 t3 = glm::vec4{ pow(pointVector[2].x,3.f), pow(pointVector[2].x,2.f), pointVector[2].x, 1.f };
		glm::vec4 t4 = glm::vec4{ pow(pointVector[3].x,3.f), pow(pointVector[3].x,2.f), pointVector[3].x, 1.f };

		glm::mat4x4 finalMatrix
		{
			t1,
			t2,
			t3,
			t4
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

	void CreateInterpolationCurve4Points(double startVal, double endingVal, double resolution, MeshActor* staticMesh) 
	{
		///Creating random numbers
		srand((unsigned) time(NULL));

		auto randomY1 = staticMesh->GetLocalPosition().x + (rand() % + 10);
		auto randomY2 = staticMesh->GetLocalPosition().x + (rand() % + 11);
		auto randomY3 = staticMesh->GetLocalPosition().x + (rand() % + 12);
		auto randomY4 = staticMesh->GetLocalPosition().x + (rand() % + 13);

		auto random1 = staticMesh->GetLocalPosition().x;
		auto random2 = staticMesh->GetLocalPosition().x;
		auto random3 = staticMesh->GetLocalPosition().x;
		auto random4 = staticMesh->GetLocalPosition().x;
		
		///Create Point in the form of a matrix to be extracted
		auto a = PointMatrix(random1,random2,random3,random4); 

		//Print the Matrix
		std::cout << "Create Point Matrix " << a << "\n";
		std::cout << "\n";

		 
		auto b = Y(a); 
		//Printing the matrix after interpolation fomrula is used
		std::cout << "INterpoltaed matrix" << b << "\n";
		

		//Matrification
		for (double i = startVal; startVal <= endingVal; i+= resolution)
		{
			P(b, i);
		}
	}



};
