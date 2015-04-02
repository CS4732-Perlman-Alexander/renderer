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

struct material
{
	std::string		name;
	struct vector3	ambient;
	struct vector3	diffuse;
	struct vector3	specular;
	int				illumination;
	float			shininess;
	float			transparencey;
	std::string		textureName;
};

struct materials
{
	std::vector<material>		materialList;
	std::vector<std::string>	textures;
};

struct mesh
{
	std::string				materialLibrary;
	std::vector<OBJObject>	objObjects;
	struct materials		objMaterials;
};
