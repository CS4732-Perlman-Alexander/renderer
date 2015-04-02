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
	std::string				name;
	std::string				material;
	std::vector<vector3>	vertices;
	std::vector<vector2>	textureCoordinates;
	std::vector<vector3>	normals;
	std::vector<face>		faces;
	bool					smoothing;
};

struct mesh
{
	std::string				materialLibrary;
	std::vector<OBJObject>	objObjects;
};