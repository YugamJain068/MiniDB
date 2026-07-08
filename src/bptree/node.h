#ifndef NODE_H
#define NODE_H

#include <vector>

class Node
{
public:
    bool isLeaf;

    std::vector<int> keys;

    Node *parent;

    Node(bool leaf) : isLeaf(leaf), parent(nullptr) {}

    virtual ~Node() = default;
};

class LeafNode : public Node
{
public:
    std::vector<int> values;

    LeafNode *next;

    LeafNode() : Node(true), next(nullptr) {}
};

class InternalNode : public Node
{
public:
    std::vector<Node *> children;

    InternalNode() : Node(false) {}
};

#endif