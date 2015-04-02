#pragma once
#include <string>
#include <vector>
#include "face.hpp"

struct vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct vector3
{
	float x;
	float y;
	float z;
};

struct vector2
{
	float x;
	float y;
};

struct OBJObject
{
	std::string					name;
	std::string					material;
	std::vector<struct vector3>	vertices;
	std::vector<struct vector2>	textureCoordinates;
	std::vector<struct vector3>	normals;
	std::vector<class face>		faces;
	bool						smoothing;
};

struct mesh
{
	std::string				materialLibrary;
	std::vector<OBJObject>	objObjects;
};