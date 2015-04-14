#pragma once
#include "node.hpp"
#include "RndrrStructures.hpp"
#include <Windows.h>
#include <string>
class nodeMesh : public Node
{
public:
	nodeMesh();
	nodeMesh(SimpleVertex* v, WORD* i, std::string fn);
	~nodeMesh();
private:
	SimpleVertex*			vertices = nullptr;			//array
	WORD*					indices = nullptr;			//array
	std::string				fileName;
};

