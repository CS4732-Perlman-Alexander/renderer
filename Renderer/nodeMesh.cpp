#include "nodeMesh.hpp"

nodeMesh::nodeMesh()
{
}

nodeMesh::nodeMesh(SimpleVertex* v, unsigned int vSize, WORD* i, unsigned int iSize, const wchar_t* fn)
{
	this->vertices = v;
	this->indices = i;
	this->fileName = fn;
	this->numVertices = vSize;
	this->numIndices = iSize;
}

nodeMesh::~nodeMesh()
{
}
