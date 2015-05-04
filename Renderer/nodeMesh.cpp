#include "nodeMesh.hpp"

nodeMesh::nodeMesh(unsigned int sI, unsigned int nI, const wchar_t* fn, std::string identifier)
{
	this->startIndices = sI;
	this->numIndices = nI;
	this->fileName = fn;
	this->meshcolor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	this->id = identifier;
	this->setType("Mesh");
}

nodeMesh::nodeMesh(unsigned int sI, unsigned int nI, DirectX::XMFLOAT4 color, std::string identifier)
{
	this->startIndices = sI;
	this->numIndices = nI;
	this->meshcolor = color;
	this->id = identifier;
	this->setType("Mesh");
}

nodeMesh::~nodeMesh()
{
}
