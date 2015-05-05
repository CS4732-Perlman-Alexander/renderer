#pragma once
#include <vector>
#include <memory>
#include <string>

class Node
{
private:
	std::shared_ptr<Node>					parent;
	std::vector<std::shared_ptr<Node>>		children;
	std::string								myType;

public:
	// Ctors, dtors.
	Node() = default;
	Node(std::shared_ptr<Node> p, std::vector<std::shared_ptr<Node>> c);
	~Node() = default;

	// Default copy ctors/assignments.
	Node(const Node&) = default;
	Node& operator=(const Node&) = default;

	// Move ctors/assignments.
	Node(Node&& n);
	Node& operator=(Node&&);

	virtual auto addChild(std::shared_ptr<Node> c, std::shared_ptr<Node> p) -> void;

	// Inline functions. They happen to  be getters and setters as well.
	virtual auto setParent(std::shared_ptr<Node> p) -> void { this->parent = p; };
	virtual auto setChildren(std::vector<std::shared_ptr<Node>> c) -> void { this->children = c; };
	virtual auto setType(std::string t)->void = 0;

	virtual auto getParent()->std::shared_ptr<Node> { return this->parent; };
	virtual auto getChildren()->std::vector<std::shared_ptr<Node>>{ return this->children; };
	virtual auto getType()->std::string = 0;
};
