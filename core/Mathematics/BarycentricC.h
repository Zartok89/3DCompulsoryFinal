#pragma once

///GLM Lib
#define GLM_ENABLE_EXPERIMENTAL 

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

///Includes
#include "actors/MeshActor.h"
#include "graphics/Mesh.h"
#include "utility/Logger.h"


class BarycentricC : Actor
{


	static double determinant(const glm::vec2& u, const glm::vec2& v)
	{
		//Calculates the determinant of the vectors
		return (u.x * v.y) - (u.y * v.x);
	}

	static double triangleArea(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C)
	{
		//Calculates the area of a triangle
		return abs(determinant(B - A, C - A) * 0.5);
	}

	static glm::vec2 centroid(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C)
	{
		return (A * B * C) / 3.f;
	}

public:
	//BarycentricC(const PawnActor* mesh) {}; 

	static std::vector<glm::vec3> getBarycentricCoordinatesActor(Mesh* mesh)  
	{
		//Itterating over n-elements as the same size as mIndices.
		//After three iterations the function skips by three
		std::vector<glm::vec3> BaryCoords{};

		assert(mesh && "Mesh not in Use");
		for (auto currentVertex = 0; currentVertex < mesh->mVertices.size(); currentVertex+= 3) 
		{
			glm::vec2 Q = { mesh->mVertices[currentVertex].mPosition.x, mesh->mVertices[currentVertex].mPosition.y };
			glm::vec2 P = { mesh->mVertices[currentVertex+1].mPosition.y, mesh->mVertices[currentVertex+1].mPosition.y };
			glm::vec2 R = { mesh->mVertices[currentVertex+2].mPosition.z, mesh->mVertices[currentVertex+2].mPosition.z };
			 
			glm::vec X = centroid(Q,P,R); 

			double areaQPR = triangleArea(Q, P, R);
			double areaXPR = triangleArea(X, P, R);
			double areaXQP = triangleArea(X, Q, P);
			double areaXQR = triangleArea(X, Q, R);
			
			double mI = areaXQR / areaQPR;  
			double mJ = areaXQR / areaQPR;
			double mK = 1.f - mI - mJ; //i+j+k should be 1    

			std::cout << "Barycentric Coordinates:" << mI << ", " << mJ << ", " << mK << std::endl;
			BaryCoords.emplace_back(currentVertex);  

		} 
		return BaryCoords; 

	}

	glm::vec3 getBarycentricCoordinates2D(const glm::vec2& Q, const glm::vec2& P, const glm::vec2& R,
		const glm::vec2& X,
		double& i, double& j, double& k)
	{
		double areaQPR = triangleArea(Q, P, R); //Total Area
		double areaXPR = triangleArea(X, P, R);
		double areaXQP = triangleArea(X, Q, P);
		double areaXQR = triangleArea(X, Q, R);

		i = areaXQR / areaQPR;
		j = areaXQR / areaQPR;
		k = 1.f - i - j; //i+j+k should be 1
	}


};
