#pragma once
#include <vector>
class Node
{
public:
	Node();
	Node(Node* p, std::vector<Node*> c);
	~Node();
	void addChild(Node* c);

	Node(const Node&) = default;
	Node& operator=(const Node&) = default;
private:
	Node*					parent;
	std::vector<Node*>		children;
};

