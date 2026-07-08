#include "b_plus_tree.h"

#include <iostream>
#include <algorithm>

BPlusTree::BPlusTree()
{
    root = nullptr;
}

bool BPlusTree::empty() const
{
    return root == nullptr;
}

void BPlusTree::printTree() const
{
    if (root == nullptr)
    {
        std::cout
            << "Tree Empty\n";

        return;
    }

    if (root->isLeaf)
    {
        LeafNode *leaf =
            static_cast<LeafNode *>(root);

        std::cout << "Leaf : ";

        for (int key : leaf->keys)
            std::cout << key << " ";

        std::cout << '\n';

        return;
    }

    InternalNode *internal =
        static_cast<InternalNode *>(root);

    std::cout
        << "Root : ";

    for (int key : internal->keys)
    {
        std::cout
            << key
            << " ";
    }

    std::cout << '\n';

    for (Node *child :
         internal->children)
    {
        LeafNode *leaf =
            static_cast<LeafNode *>(child);

        std::cout
            << "[ ";

        for (int key : leaf->keys)
            std::cout
                << key
                << " ";

        std::cout
            << "] ";
    }
    std::cout << '\n';
}

LeafNode *BPlusTree::findLeaf(int key)
{
    if (root == nullptr)
        return nullptr;

    Node *current = root;

    while (!current->isLeaf)
    {
        InternalNode *internal = static_cast<InternalNode *>(current);

        int i = 0;

        while (i < internal->keys.size() &&
               key >= internal->keys[i])
        {
            i++;
        }

        current = internal->children[i];
    }

    return static_cast<LeafNode *>(current);
}

void BPlusTree::insert(int key)
{
    if (root == nullptr)
    {
        LeafNode *leaf = new LeafNode();

        leaf->keys.push_back(key);

        leaf->values.push_back(key);

        root = leaf;

        return;
    }

    LeafNode *leaf = findLeaf(key);

    auto keyPos =
        std::lower_bound(
            leaf->keys.begin(),
            leaf->keys.end(),
            key);

    int index = keyPos - leaf->keys.begin();

    leaf->keys.insert(
        keyPos,
        key);

    leaf->values.insert(
        leaf->values.begin() + index,
        key);
    if (leaf->keys.size() > ORDER)
    {
        splitLeaf(leaf);
    }
}

bool BPlusTree::search(int key)
{
    LeafNode *leaf = findLeaf(key);

    if (leaf == nullptr)
        return false;

    return std::binary_search(
        leaf->keys.begin(),
        leaf->keys.end(),
        key);
}

void BPlusTree::splitLeaf(LeafNode *leaf)
{
    LeafNode *newLeaf = new LeafNode();
    int mid = leaf->keys.size() / 2;

    newLeaf->keys.assign(
        leaf->keys.begin() + mid,
        leaf->keys.end());

    newLeaf->values.assign(
        leaf->values.begin() + mid,
        leaf->values.end());

    leaf->keys.resize(mid);
    leaf->values.resize(mid);

    newLeaf->next = leaf->next;
    leaf->next = newLeaf;

    insertIntoParent(
        leaf,
        newLeaf->keys.front(),
        newLeaf);
}

void BPlusTree::printLeaves() const
{
    if (root == nullptr)
        return;

    Node *current = root;

    while (!current->isLeaf)
    {
        current =
            static_cast<InternalNode *>(current)
                ->children.front();
    }

    LeafNode *leaf =
        static_cast<LeafNode *>(current);

    while (leaf)
    {
        std::cout << "[ ";

        for (int key : leaf->keys)
            std::cout << key << " ";

        std::cout << "] -> ";

        leaf = leaf->next;
    }

    std::cout << "nullptr\n";
}

void BPlusTree::insertIntoParent(Node *left,
                                 int key,
                                 Node *right)
{
    if (left == root)
    {
        InternalNode *newRoot = new InternalNode();

        newRoot->keys.push_back(key);

        newRoot->children.push_back(left);
        newRoot->children.push_back(right);

        left->parent = newRoot;
        right->parent = newRoot;

        root = newRoot;

        return;
    }

    InternalNode *parent =
        static_cast<InternalNode *>(left->parent);

    int childIndex = 0;

    while (parent->children[childIndex] != left)
    {
        childIndex++;
    }

    parent->keys.insert(
        parent->keys.begin() + childIndex,
        key);

    parent->children.insert(
        parent->children.begin() + childIndex + 1,
        right);

    right->parent = parent;

    if (parent->keys.size() > ORDER)
    {
        splitInternalNode(parent);
    }

}

void BPlusTree::splitInternalNode(InternalNode *node)
{
    InternalNode *newInternal = new InternalNode();
    int mid = node->keys.size() / 2;
    int promotedKey = node->keys[mid];

    newInternal->keys.assign(node->keys.begin() + mid + 1, node->keys.end());
    node->keys.resize(mid);

    newInternal->children.assign(
        node->children.begin() + mid + 1,
        node->children.end());

    node->children.resize(mid + 1);
    for (Node *child : newInternal->children)
        child->parent = newInternal;

    insertIntoParent(node, promotedKey, newInternal);
}