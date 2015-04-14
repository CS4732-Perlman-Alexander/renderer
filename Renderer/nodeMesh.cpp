#include "nodeMesh.hpp"

nodeMesh::nodeMesh()
{
}

nodeMesh::nodeMesh(SimpleVertex* v, WORD* i, std::string fn)
{
	this->vertices = v;
	this->indices = i;
	this->fileName = fn;
}

nodeMesh::~nodeMesh()
{
}
