#include "nodeLight.hpp"

nodeLight::nodeLight()
{
}

nodeLight::nodeLight(lightAmbient* a, lightDiffuse* d, lightSpecular* s)
{
	this->ambient = a;
	this->diffuse = d;
	this->specular = s;
}

nodeLight::~nodeLight()
{
}
