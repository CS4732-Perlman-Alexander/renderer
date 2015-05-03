#include "nodeMesh.hpp"

nodeMesh::nodeMesh()
{
}

nodeMesh::nodeMesh(unsigned int sI, unsigned int nI, const wchar_t* fn)
{
	this->startIndices = sI;
	this->numIndices = nI;
	this->fileName = fn;
	this->setType("Mesh");
}

nodeMesh::~nodeMesh()
{
}
