#include "b_plus_tree.h"

#include <iostream>
#include <algorithm>
#include <climits>

BPlusTree::BPlusTree(BufferPoolManager *bpm)
{
    root = nullptr;
    bufferPool = bpm;
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

void BPlusTree::insert(int key, const RecordPointer &pointer)
{
    if (root == nullptr)
    {
        LeafNode *leaf = new LeafNode();

        leaf->keys.push_back(key);

        leaf->values.push_back(pointer);

        root = leaf;

        return;
    }

    LeafNode *leaf = findLeaf(key);
    auto duplicate =
        std::lower_bound(
            leaf->keys.begin(),
            leaf->keys.end(),
            key);

    if (duplicate != leaf->keys.end() &&
        *duplicate == key)
    {
        throw std::runtime_error("Duplicate key");
    }

    auto keyPos = std::lower_bound(
        leaf->keys.begin(),
        leaf->keys.end(),
        key);

    int index = keyPos - leaf->keys.begin();

    leaf->keys.insert(keyPos, key);

    leaf->values.insert(
        leaf->values.begin() + index,
        pointer);

    if (leaf->keys.size() > ORDER)
    {
        splitLeaf(leaf);
    }
    std::cout << "\nAfter inserting " << key << '\n';

    std::cout
        << "Tree Height = "
        << height()
        << '\n';
    if (!verifyTree())
    {
        std::cout
            << "B+ TREE CORRUPTED AFTER INSERTING "
            << key
            << '\n';

        return;
    }
}
RecordPointer BPlusTree::search(int key)
{
    LeafNode *leaf = findLeaf(key);

    if (leaf == nullptr)
        return RecordPointer();

    auto it =
        std::lower_bound(
            leaf->keys.begin(),
            leaf->keys.end(),
            key);

    if (it == leaf->keys.end() ||
        *it != key)
    {
        return RecordPointer();
    }

    int index =
        it -
        leaf->keys.begin();

    return leaf->values[index];
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

        for (size_t i = 0; i < leaf->keys.size(); i++)
        {
            std::cout
                << leaf->keys[i]
                << "("
                << leaf->values[i].pageId
                << ","
                << leaf->values[i].slotId
                << ") ";
        }

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
bool BPlusTree::verifyTree() const
{
    if (root == nullptr)
        return true;

    int leafDepth = -1;

    bool structureOK =
        verifyNode(
            root,
            LLONG_MIN,
            true,
            LLONG_MAX,
            true,
            0,
            leafDepth);

    if (!structureOK)
        return false;

    if (!verifyLeafChain())
        return false;

    return true;
}

bool BPlusTree::verifyNode(
    Node *node,
    long long minKey,
    bool minInclusive,
    long long maxKey,
    bool maxInclusive,
    int depth,
    int &leafDepth) const
{
    if (node == nullptr)
        return false;

    // ========================================================
    // LEAF
    // ========================================================

    if (node->isLeaf)
    {
        LeafNode *leaf =
            static_cast<LeafNode *>(node);

        // Keys and values must have same size
        if (leaf->keys.size() !=
            leaf->values.size())
        {
            std::cout
                << "ERROR: Leaf keys/values mismatch\n";

            return false;
        }

        // Leaf keys must be sorted
        for (size_t i = 1;
             i < leaf->keys.size();
             i++)
        {
            if (leaf->keys[i - 1] >=
                leaf->keys[i])
            {
                std::cout
                    << "ERROR: Leaf keys not strictly sorted\n";

                return false;
            }
        }

        // All keys must be inside allowed range
        for (int key : leaf->keys)
        {
            if (minInclusive)
            {
                if (key < minKey)
                    return false;
            }
            else
            {
                if (key <= minKey)
                    return false;
            }

            if (maxInclusive)
            {
                if (key > maxKey)
                    return false;
            }
            else
            {
                if (key >= maxKey)
                    return false;
            }
        }

        // Every leaf must be at same depth
        if (leafDepth == -1)
        {
            leafDepth = depth;
        }
        else if (leafDepth != depth)
        {
            std::cout
                << "ERROR: Leaves have different depths\n";

            return false;
        }

        return true;
    }

    // ========================================================
    // INTERNAL NODE
    // ========================================================

    InternalNode *internal =
        static_cast<InternalNode *>(node);

    // Internal node invariant
    if (internal->children.size() !=
        internal->keys.size() + 1)
    {
        std::cout
            << "ERROR: Internal node children != keys + 1\n";

        return false;
    }

    // Internal keys must be strictly sorted
    for (size_t i = 1;
         i < internal->keys.size();
         i++)
    {
        if (internal->keys[i - 1] >=
            internal->keys[i])
        {
            std::cout
                << "ERROR: Internal keys not sorted\n";

            return false;
        }
    }

    // Parent pointer check
    for (Node *child :
         internal->children)
    {
        if (child == nullptr)
        {
            std::cout
                << "ERROR: Null child\n";

            return false;
        }

        if (child->parent != node)
        {
            std::cout
                << "ERROR: Incorrect parent pointer\n";

            return false;
        }
    }

    // ========================================================
    // Recursively verify children
    // ========================================================

    for (size_t i = 0;
         i < internal->children.size();
         i++)
    {
        long long childMin = minKey;
        long long childMax = maxKey;

        bool childMinInclusive =
            minInclusive;

        bool childMaxInclusive =
            maxInclusive;

        if (i > 0)
        {
            childMin =
                internal->keys[i - 1];

            childMinInclusive = true;
        }

        if (i < internal->keys.size())
        {
            childMax =
                internal->keys[i];

            childMaxInclusive = false;
        }

        if (!verifyNode(
                internal->children[i],
                childMin,
                childMinInclusive,
                childMax,
                childMaxInclusive,
                depth + 1,
                leafDepth))
        {
            return false;
        }
    }

    return true;
}

int BPlusTree::height() const
{
    if (root == nullptr)
        return 0;

    int height = 0;

    Node *current = root;

    while (current != nullptr)
    {
        height++;

        if (current->isLeaf)
            break;

        InternalNode *internal =
            static_cast<InternalNode *>(current);

        current =
            internal->children.front();
    }

    return height;
}

void BPlusTree::printRootInfo() const
{
    if (root == nullptr)
    {
        std::cout << "Root: NULL\n";
        return;
    }

    std::cout
        << "Root is "
        << (root->isLeaf ? "Leaf" : "Internal")
        << '\n';

    if (!root->isLeaf)
    {
        InternalNode *internal =
            static_cast<InternalNode *>(root);

        std::cout
            << "Root Keys: ";

        for (int key : internal->keys)
        {
            std::cout
                << key
                << " ";
        }

        std::cout << '\n';

        std::cout
            << "Root Children: "
            << internal->children.size()
            << '\n';
    }
}

bool BPlusTree::verifyParentPointers(Node *node) const
{
    if (node == nullptr ||
        node->isLeaf)
    {
        return true;
    }

    InternalNode *internal =
        static_cast<InternalNode *>(node);

    for (Node *child : internal->children)
    {
        if (child->parent != node)
            return false;

        if (!verifyParentPointers(child))
            return false;
    }

    return true;
}

bool BPlusTree::verifyInternalStructure(Node *node) const
{
    if (node == nullptr ||
        node->isLeaf)
    {
        return true;
    }

    InternalNode *internal =
        static_cast<InternalNode *>(node);

    if (internal->children.size() !=
        internal->keys.size() + 1)
    {
        return false;
    }

    for (Node *child : internal->children)
    {
        if (!verifyInternalStructure(child))
            return false;
    }

    return true;
}

bool BPlusTree::verifyLeafChain() const
{
    if (root == nullptr)
        return true;

    Node *current = root;

    while (!current->isLeaf)
    {
        InternalNode *internal =
            static_cast<InternalNode *>(current);

        current =
            internal->children.front();
    }

    LeafNode *leaf =
        static_cast<LeafNode *>(current);

    bool firstKey = true;
    int previousKey = 0;

    while (leaf != nullptr)
    {
        for (int key : leaf->keys)
        {
            if (!firstKey &&
                key <= previousKey)
            {
                std::cout
                    << "ERROR: Leaf chain not sorted\n";

                return false;
            }

            previousKey = key;
            firstKey = false;
        }

        leaf = leaf->next;
    }

    return true;
}