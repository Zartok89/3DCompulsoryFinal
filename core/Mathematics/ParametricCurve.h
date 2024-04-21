#pragma once
#include <iostream>
#include <string>
#include <actors/Actor.h>
#include <graphics/Mesh.h>
#include <graphics/Vertex.h>
#include <glm/glm.hpp>

class ParametricCurve 
{
	

public:
	std::vector<Vertex> F; 

	Mesh* CreateInterpolationCurve3Points(const double& startVal, const double& endingVal, const double& resolution) 
	{
		///Creating random numbers
		srand((unsigned) time(NULL));

	
		//Creating an array of vec2/points
		std::vector<glm::vec2> Points
		{
			glm::vec2(0,0),
			glm::vec2(1,1),
			glm::vec2(5,2)
		};

		//Creating variables for matrix A and vector b
		glm::mat3x3 A; 
		glm::vec3 b;
	
		//Iterating over Points.size and adding respective values to respective variable
		for (auto i = 0; i < Points.size(); i++)
		{
			A[0][i] = pow(Points[i].x, 2); 
			A[1][i] = Points[i].x; 
			A[2][i] = 1.f; 

			b[i] = Points[i].y; 

		} 
		
		//GetInverse of A
		auto getInverse = glm::inverse(A); 
		auto getX = getInverse * b; 
		 
		//Crating a array of Vertices
		//std::vector<Vertex> F; 
		std::vector<unsigned int> Indices;

		for (double x = startVal; x <= endingVal; x += resolution)
		{
			double calcF = (getX.x * pow(x, 2)) + (getX.y * x) + (getX.z);
			F.emplace_back(x, 0, calcF); 
		}

		for (int i = 0; i < F.size(); i++)
		{
			Indices.emplace_back(i);
		}

		return new Mesh("InterpolationC", std::move(F), std::move(Indices), nullptr);

	}


};
