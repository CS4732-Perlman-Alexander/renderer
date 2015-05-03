#include "Node.hpp"

Node::Node(std::shared_ptr<Node> p, std::vector<std::shared_ptr<Node>> c) : parent(p), children(c)
{
};

Node::Node(Node&& n)
{
	this->setChildren(std::move(n.getChildren()));
	this->setParent(std::move(n.getParent()));
}

Node& Node::operator=(Node&& n)
{
	this->setChildren(std::move(n.getChildren()));
	this->setParent(std::move(n.getParent()));
	return *this;
}

auto Node::addChild(std::shared_ptr<Node> c, std::shared_ptr<Node> p) -> void
{
	c.get()->setParent(p);
	this->children.emplace_back(c);
}
