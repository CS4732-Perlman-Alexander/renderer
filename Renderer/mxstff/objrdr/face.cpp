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

face::face(vector3& pX, vector3& pY, vector3& pZ)
{
	*posX = pX;
	*posY = pY;
	*posZ = pZ;
	tcX = nullptr;
	tcY = nullptr;
	tcZ = nullptr;
	normX = nullptr;
	normY = nullptr;
	normZ = nullptr;
}

face::face(vector3& pX, vector3& pY, vector3& pZ, vector2& tX, vector2& tY, vector2& tZ)
{
	*posX = pX;
	*posY = pY;
	*posZ = pZ;
	*tcX = tX;
	*tcY = tY;
	*tcZ = tZ;
	normX = nullptr;
	normY = nullptr;
	normZ = nullptr;
}
face::face(vector3& pX, vector3& pY, vector3& pZ, vector2& tX, vector2& tY, vector2& tZ, vector3& nX, vector3& nY, vector3& nZ)
{
	*posX = pX;
	*posY = pY;
	*posZ = pZ;
	*tcX = tX;
	*tcY = tY;
	*tcZ = tZ;
	*normX = nX;
	*normY = nY;
	*normZ = nZ;
}

face::~face()
{
}