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
#include "actors/PawnActor.h"
#include "actors/StaticMeshActor.h"
#include "graphics/Mesh.h"
#include "utility/Logger.h"

class StaticMeshActor;
class PawnActor;
class Mesh;

class BarycentricC
{


	double determinant(const glm::vec2& u, const glm::vec2& v)
	{
		//Calculates the determinant of the vectors
		return (u.x * v.y) - (u.y * v.x);
	}

	double trinagleArea(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C)
	{
		//Calculates the area of a triangle
		return abs(determinant(B - A, C - A) * 0.5);
	}

	glm::vec2 centroid(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C)
	{
		return (A * B * C) / 3.f;
	}

public:
	//BarycentricC(const PawnActor* mesh) {}; 

	void getBarycentricCoordinatesActor(const PawnActor* mesh) 
	{
		//Itterating over n-elements as the same size as mIndices.
		//After three iterations the function skips by three

		assert(mesh && "PawnActor not in Use");
		//for (auto currentVertex = 0; currentVertex < mesh->mMesh->mVertices.size(); currentVertex+= 3) 
		//{
		//	glm::vec2 Q = { mesh->mMesh->mVertices[currentVertex].mPosition.x,mesh->mMesh->mVertices[currentVertex].mPosition.y }; 
		//	glm::vec2 P = { mesh->mMesh->mVertices[currentVertex+1].mPosition.y,mesh->mMesh->mVertices[currentVertex+1].mPosition.y }; 
		//	glm::vec2 R = { mesh->mMesh->mVertices[currentVertex+2].mPosition.z,mesh->mMesh->mVertices[currentVertex+2].mPosition.z }; 
		//	 
		//	glm::vec2 X = centroid(Q,P,R); 

		//	double areaQPR = trinagleArea(Q, P, R);
		//	double areaXPR = trinagleArea(X, P, R);
		//	double areaXQP = trinagleArea(X, Q, P);
		//	double areaXQR = trinagleArea(X, Q, R); 
		//	
		//	mI = areaXQR / areaQPR;  
		//	mJ = areaXQR / areaQPR;  
		//	mK = 1.f - mI - mK; //i+j+k should be 1  

		//	std::cout << "Barycentric Coordinates:" << mI << ", " << mJ << ", " << mK << std::endl;
		//} 

	}

	glm::vec3 getBarycentricCoordinates2D(const glm::vec2& Q, const glm::vec2& P, const glm::vec2& R,
		const glm::vec2& X,
		double& i, double& j, double& k)
	{
		double areaQPR = trinagleArea(Q, P, R); //Total Area
		double areaXPR = trinagleArea(X, P, R); 
		double areaXQP = trinagleArea(X, Q, P); 
		double areaXQR = trinagleArea(X, Q, R);

		i = areaXQR / areaQPR;
		j = areaXQR / areaQPR;
		k = 1.f - i - j; //i+j+k should be 1
	}


private:
	double mI;
	double mJ;
	double mK;
};
