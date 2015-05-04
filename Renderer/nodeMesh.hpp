#pragma once
#include "node.hpp"
#include "RndrrStructures.hpp"
#include <Windows.h>

class nodeMesh : public Node
{
private:
	unsigned int			startIndices;
	unsigned int			numIndices;
	const wchar_t*			fileName;
	std::string				myType;
	DirectX::XMFLOAT4		meshcolor;
	std::string				id;

public:
	nodeMesh(){};
	nodeMesh(unsigned int sI, unsigned int nI, const wchar_t* fn, std::string identifier);
	nodeMesh(unsigned int sI, unsigned int nI, DirectX::XMFLOAT4 color, std::string identifier);
	~nodeMesh();

	nodeMesh(const nodeMesh&) = default;
	nodeMesh& operator=(const nodeMesh&) = default;

	auto getStartIndices()->unsigned int { return startIndices; };
	auto getNumIndices()->unsigned int { return numIndices; };

	auto getType()->std::string { return this->myType; };
	auto setType(std::string t)->void { this->myType = t; };

	auto getMeshColor() -> DirectX::XMFLOAT4 { return this->meshcolor; };
	auto getID()->std::string { return id; };

};

