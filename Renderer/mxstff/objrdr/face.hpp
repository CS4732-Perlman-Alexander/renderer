#pragma once
#include "structures.hpp"
class face
{
public:
	struct vector3 *posX;
	struct vector3 *posY;
	struct vector3 *posZ;
	struct vector2 *tcX;
	struct vector2 *tcY;
	struct vector2 *tcZ;
	struct vector3 *normX;
	struct vector3 *normY;
	struct vector3 *normZ;

	face();
	face(struct vector3& pX, struct vector3& pY, struct vector3& pZ);
	face(struct vector3& pX, struct vector3& pY, struct vector3& pZ, struct vector2& tX, struct vector2& tY, struct vector2& tZ);
	face(struct vector3& pX, struct vector3& pY, struct vector3& pZ, struct vector2& tX, struct vector2& tY, struct vector2& tZ, struct vector3& nX, struct vector3& nY, struct vector3& nZ);
	~face();
};