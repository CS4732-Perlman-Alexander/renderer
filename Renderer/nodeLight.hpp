#pragma once
#include "node.hpp"
#include "lightStructs.hpp"
class nodeLight : public Node
{
public:
	nodeLight();
	nodeLight(lightAmbient* a, lightDiffuse* d, lightSpecular* s);
	~nodeLight();
	auto getType()->std::string{ return this->myType; };
	auto setType(std::string t)->void{ this->myType = t; };
private:
	lightAmbient* ambient;
	lightDiffuse* diffuse;
	lightSpecular* specular;
	std::string		myType;
};

