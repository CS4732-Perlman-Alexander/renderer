#pragma once
#include "node.hpp"
#include "lightStructs.hpp"
class nodeLight : public Node
{
public:
	nodeLight();
	nodeLight(lightAmbient* a, lightDiffuse* d, lightSpecular* s);
	~nodeLight();
private:
	lightAmbient* ambient;
	lightDiffuse* diffuse;
	lightSpecular* specular;
};

