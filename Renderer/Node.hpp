#pragma once
#include <vector>
#include <memory>
class Node
{
private:
	std::shared_ptr<Node>					parent;
	std::vector<std::shared_ptr<Node>>		children;

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

	auto addChild(std::shared_ptr<Node> c) -> void;

	// Inline functions. They happen to  be getters and setters as well.
	auto setParent(std::shared_ptr<Node> p) -> void { this->parent = p; };
	auto setChildren(std::vector<std::shared_ptr<Node>> c) -> void { this->children = c; };
	auto getParent()->std::shared_ptr<Node> { return this->parent; };
	auto getChildren()->std::vector<std::shared_ptr<Node>>{ return this->children; };
};
