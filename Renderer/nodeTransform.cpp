#include "nodeTransform.hpp"

nodeTransform::nodeTransform()
{
}

nodeTransform::nodeTransform(DirectX::XMMATRIX *t)
{
	this->transform = t;
}

nodeTransform::~nodeTransform()
{
}
