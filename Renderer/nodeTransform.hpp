#pragma once
#include "node.hpp"
#include <DirectXMath.h>
#include <functional>
class nodeTransform : public Node
{
private:
	std::function<DirectX::XMMATRIX(float)> tForm;
	std::string	myType;
	std::string id;

public:
	nodeTransform() = delete;
	~nodeTransform() = default;

	nodeTransform(std::function<DirectX::XMMATRIX(float)> tf, std::string identifier)
	{
		tForm = tf;
		id = identifier;
		setType("Transform");
	}

	auto applyTransformation(float time) -> DirectX::XMMATRIX
	{
		return tForm(time);
	}

	auto getType()->std::string{ return this->myType; };
	auto setType(std::string t)->void{ this->myType = t; };
	auto getID()->std::string { return id; };
};