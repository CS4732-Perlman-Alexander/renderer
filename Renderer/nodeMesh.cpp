#include "nodeMesh.hpp"

nodeMesh::nodeMesh(unsigned int sI, unsigned int nI, const wchar_t* fn)
{
	this->startIndices = sI;
	this->numIndices = nI;
	this->fileName = fn;
	this->meshcolor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	this->setType("Mesh");
}

nodeMesh::nodeMesh(unsigned int sI, unsigned int nI, DirectX::XMFLOAT4 color)
{
	this->startIndices = sI;
	this->numIndices = nI;
	this->meshcolor = color;
	this->setType("Mesh");
}

nodeMesh::~nodeMesh()
{
}
