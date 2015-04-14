#include "Node.hpp"

Node::Node()
{
};

Node::Node(Node* p, std::vector<Node*> c)
{
	this->parent = p;
	this->children = c;
};

void Node::addChild(Node* c)
{
	c->parent = this;
	this->children.push_back(c);
}

Node::~Node()
{
};
