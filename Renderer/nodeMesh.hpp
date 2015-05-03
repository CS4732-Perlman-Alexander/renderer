#pragma once
#include "node.hpp"
#include "RndrrStructures.hpp"
#include <Windows.h>
class nodeMesh : public Node
{
public:
	nodeMesh();
	//nodeMesh(SimpleVertex* v, unsigned int vSize, WORD* i, unsigned int iSize, const wchar_t* fn);
	nodeMesh(unsigned int sI, unsigned int nI, const wchar_t* fn);
	~nodeMesh();

	nodeMesh(const nodeMesh&) = default;
	nodeMesh& operator=(const nodeMesh&) = default;

	auto getStartIndices()->unsigned int{ return startIndices; }
	auto getNumIndices()->unsigned int{ return numIndices; }

	auto getType()->std::string { return this->myType; };
	auto setType(std::string t)->void{ this->myType = t; };
private:
	unsigned int			startIndices;
	unsigned int			numIndices;
	const wchar_t*			fileName;
	std::string				myType;
};

