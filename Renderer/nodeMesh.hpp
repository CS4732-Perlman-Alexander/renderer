#pragma once
#include "node.hpp"
#include "RndrrStructures.hpp"
#include <Windows.h>
class nodeMesh : public Node
{
public:
	nodeMesh();
	nodeMesh(SimpleVertex* v, unsigned int vSize, WORD* i, unsigned int iSize, const wchar_t* fn);
	~nodeMesh();

	nodeMesh(const nodeMesh&) = default;
	nodeMesh& operator=(const nodeMesh&) = default;

	auto getVertices()->SimpleVertex*{ return vertices; }
	auto getIndices()->WORD*{ return indices; }
	auto getNumVertices()->unsigned int{ return numVertices; }
	auto getNumIndices()->unsigned int{ return numIndices; }
private:
	SimpleVertex*			vertices = nullptr;			// points to the front of an array
	unsigned int			numVertices;
	WORD*					indices = nullptr;			// points to the front of an array
	unsigned int			numIndices;
	const wchar_t*			fileName;
};

