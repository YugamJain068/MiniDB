#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "node.h"

#include "../buffer/buffer_pool_manager.h"
constexpr int ORDER = 4;

class BPlusTree
{
private:
    Node *root;
    BufferPoolManager *bufferPool;
    bool verifyNode(
        Node *node,
        long long minKey,
        bool minInclusive,
        long long maxKey,
        bool maxInclusive,
        int depth,
        int &leafDepth) const;

    bool verifyLeafChain() const;

    bool verifyAllKeys() const;

public:
    BPlusTree(BufferPoolManager *bpm);

    bool empty() const;
    void printTree() const;

    void insert(int key, const RecordPointer &pointer);
    RecordPointer search(int key);

    LeafNode *findLeaf(int key);
    void splitLeaf(LeafNode *leaf);
    void printLeaves() const;
    void insertIntoParent(Node *left, int key, Node *right);
    void splitInternalNode(InternalNode *node);
    bool verifyTree() const;
    int height() const;
    void printRootInfo() const;
    bool verifyParentPointers(Node *node) const;
    bool verifyInternalStructure(Node *node) const;
    Node *getRoot() const
    {
        return root;
    }
};

#endif