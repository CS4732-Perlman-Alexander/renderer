#include "face.hpp"

face::face()
{
	posX = new vector3();
	posY = new vector3();
	posZ = new vector3();
	tcX = nullptr;
	tcY = nullptr;
	tcZ = nullptr;
	normX = nullptr;
	normY = nullptr;
	normZ = nullptr;
}

face::face(struct vector3& pX, struct vector3& pY, struct vector3& pZ)
{
	posX = &pX;
	posY = &pY;
	posZ = &pZ;
	tcX = nullptr;
	tcY = nullptr;
	tcZ = nullptr;
	normX = nullptr;
	normY = nullptr;
	normZ = nullptr;
}

face::face(struct vector3& pX, struct vector3& pY, struct vector3& pZ, struct vector2& tX, struct vector2& tY, struct vector2& tZ)
{
	posX = &pX;
	posY = &pY;
	posZ = &pZ;
	tcX = &tX;
	tcY = &tY;
	tcZ = &tZ;
	normX = nullptr;
	normY = nullptr;
	normZ = nullptr;
}
face::face(struct vector3& pX, struct vector3& pY, struct vector3& pZ, struct vector2& tX, struct vector2& tY, struct vector2& tZ, struct vector3& nX, struct vector3& nY, struct vector3& nZ)
{
	posX = &pX;
	posY = &pY;
	posZ = &pZ;
	tcX = &tX;
	tcY = &tY;
	tcZ = &tZ;
	normX = &nX;
	normY = &nY;
	normZ = &nZ;
}

face::~face()
{
}