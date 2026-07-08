#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "node.h"
constexpr int ORDER = 4;

class BPlusTree
{
private:
    Node *root;

public:
    BPlusTree();

    bool empty() const;
    void printTree() const;

    void insert(int key);
    bool search(int key);

    LeafNode *findLeaf(int key);
    void splitLeaf(LeafNode *leaf);
    void printLeaves() const;
    void insertIntoParent(Node *left, int key, Node *right);
    void splitInternalNode(InternalNode* node);
};

#endif