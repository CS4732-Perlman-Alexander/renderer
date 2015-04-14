#pragma once
#include "node.hpp"
#include <DirectXMath.h>
class nodeTransform : public Node
{
public:
	nodeTransform();
	nodeTransform(DirectX::XMMATRIX *t);
	~nodeTransform();
private:
	DirectX::XMMATRIX*		transform = nullptr;
};

