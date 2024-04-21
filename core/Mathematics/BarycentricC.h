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
#include "utility/AssimpLoader.h"
#include "utility/Defines.h" 
#include <vector>
#include <cmath>

class BarycentricC : Actor 
{
	///Barycentric functions
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

	///Plane Functions
	///Expression
	///f(x,y) = x^3 + y^2 + pi(x*y) + pi
	 

	/*static double xf(const double xIn)
	{
		return xIn;
	}
	static double yf(const double yIn)
	{
		return yIn;
	}
	static double zf(const double zIn)
	{
		const double pi = glm::pi<double>(); 
		return pow(xf(zIn), 3) + pow(yf(zIn), 2) +  pi*(xf(zIn) * yf(zIn)) + pi;  
	}*/
public:

	static std::vector<glm::vec3> getBarycentricCoordinatesActor(const std::string fbx_Path, const Mesh* mesh)
	{
		//Itterating over n-elements as the same size as mIndices.
		//After three iterations the function skips by three
		std::vector<glm::vec3> BaryCoords{};
		/*mesh->Load(SOURCE_DIRECTORY + fbx_Path); */

		assert(mesh && "Mesh not in Use");
	
		for (auto currentVertex = 0; currentVertex < mesh->mVertices.size(); currentVertex+= 3) 
		{
			glm::vec2 Q = { mesh->mVertices[currentVertex].mPosition.x,    mesh->mVertices[currentVertex].mPosition.y };
			glm::vec2 P = { mesh->mVertices[currentVertex+1].mPosition.y,  mesh->mVertices[currentVertex+1].mPosition.y };
			glm::vec2 R = { mesh->mVertices[currentVertex+2].mPosition.z,  mesh->mVertices[currentVertex+2].mPosition.z };
			 
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

	static Mesh* CreatePlane(const double& xMin, const double& zMin, const double& xMax, const double& zMax, const double& resolution)
	{
		std::vector<Vertex> mPlaneVert;
		std::vector<unsigned int> getInidces;   
		double y;
		double pi = 2*glm::acos(0.0f);  

		for (auto x = zMin; x < zMax; x += resolution)
		{
			for (auto z = zMin; z < zMax; z += resolution)
			{
				///Lower Triangle
				y = glm::cos(4*x)*glm::cos(2*z)+pi; //Bottom Left 
				mPlaneVert.emplace_back(x, y, z);

				y = glm::cos(4 * (x+resolution)) * glm::cos(2 * z) + pi; //Bottom Right 
				mPlaneVert.emplace_back(x + resolution, y, z);

				y = glm::cos(4 * x) * glm::cos(2 * (z+resolution)) + pi; //Top Left 
				mPlaneVert.emplace_back(x, y, z + resolution);


				///Upper Triangle
				y = glm::cos(4 * x) * glm::cos(2 * (z + resolution)) + pi; //Top Left 
				mPlaneVert.emplace_back(x, y, z + resolution);

				y = glm::cos(4 * (x + resolution)) * glm::cos(2 * z) + pi; //Bottom Right
				mPlaneVert.emplace_back(x + resolution, y, z);

				y = glm::cos(4 * (x + resolution)) * glm::cos(2 * (z+resolution)) + pi; //Top Rigth
				mPlaneVert.emplace_back(x+resolution, y, z+resolution);  

			
			}
		}

		for (int i = 0; i < mPlaneVert.size(); i+= 3)
		{
			getInidces.emplace_back(i);
			getInidces.emplace_back(i+1);
			getInidces.emplace_back(i+2);

		}

		return new Mesh("Plane2D", std::move(mPlaneVert), std::move(getInidces), nullptr); 
	}

};
