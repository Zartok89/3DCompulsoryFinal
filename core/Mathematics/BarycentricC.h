#pragma once
#include <glm/glm.hpp>

class BarycentricC
{
	glm::vec2 Cross2D(const glm::vec2 A, const glm::vec2 B)
	{
		float a = A.x * B.y;//Ax * By
		float b = A.y * B.x;
		glm::vec2 Cross = { a,-b };
		return Cross;
	}

public:
	virtual glm::vec3 BarycentricFormula(const glm::vec2& Q, const glm::vec2& P, const glm::vec2& R)
	{
		//Calculating new vectors from vector substraction
		glm::vec2 PQ = P - Q;
		glm::vec2 QR = R - Q;
		glm::vec2 PR = R - P;
		
		//glm::vec3 n = Cross2D(PQ, QR); 
		glm::vec2 n = Cross2D(PQ, QR);
		float areaPQR = n.length(); //double the area

		glm::vec3 baryc{};

		//Calc Centre(X)
		glm::vec2 centreArea = Cross2D(P, Q);
		float X = centreArea.length() * 0.5;

		//Calc u
		glm::vec2 p = P - X;
		glm::vec2 q = R - X;
		n = Cross2D(p, q);
		baryc.x = 1 / areaPQR;

		//Calc v
		p = R - X;
		q = P - X;
		n = Cross2D(p, q);
		baryc.y = 1 / areaPQR;

		//Calc w
		p = R - X;
		q = Q - X;
		n = Cross2D(p, q);
		baryc.z = 1 / areaPQR;

		return baryc;
	};


};