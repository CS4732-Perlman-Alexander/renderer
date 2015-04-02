#pragma once
#include "structures.hpp"
class face
{
public:
	vector3 *posX;
	vector3 *posY;
	vector3 *posZ;
	vector2 *tcX;
	vector2 *tcY;
	vector2 *tcZ;
	vector3 *normX;
	vector3 *normY;
	vector3 *normZ;

	face();
	face(vector3& pX, vector3& pY, vector3& pZ);
	face(vector3& pX, vector3& pY, vector3& pZ, vector2& tX, vector2& tY, vector2& tZ);
	face(vector3& pX, vector3& pY, vector3& pZ, vector2& tX, vector2& tY, vector2& tZ, vector3& nX, vector3& nY, vector3& nZ);
	~face();
};