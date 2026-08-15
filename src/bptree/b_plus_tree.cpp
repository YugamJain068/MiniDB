#include "b_plus_tree.h"

#include <iostream>
#include <algorithm>
#include <climits>

BPlusTree::BPlusTree(
    BufferPoolManager *bpm,
    const std::string &filename)
    : bufferPool(bpm),
      pageManager(*bpm, filename),
      rootPageId(-1)
{
}

bool BPlusTree::initialize()
{
    if (!pageManager.initializeIndex())
        return false;

    BPlusTreeMetadata metadata{};

    if (!pageManager.readMetadata(metadata))
        return false;

    rootPageId = metadata.rootPageId;

    return true;
}

bool BPlusTree::empty() const
{
    return rootPageId == -1;
}

// bool BPlusTree::empty() const
// {
//     return root == nullptr;
// }

int BPlusTree::getRootPageId() const
{
    return rootPageId;
}

bool BPlusTree::readRootLeaf(
    LeafPage &leaf)
{
    if (rootPageId == -1)
        return false;

    return pageManager.readLeafPage(
        rootPageId,
        leaf);
}

bool BPlusTree::insert(
    int key,
    const RecordPointer &pointer)
{
    // =========================================
    // CASE 1: Empty tree
    // =========================================

    if (empty())
    {
        int pageId =
            pageManager.allocateLeafPage();

        if (pageId == -1)
            return false;

        LeafPage leaf{};

        if (!pageManager.readLeafPage(
                pageId,
                leaf))
        {
            return false;
        }

        leaf.header.size = 1;

        leaf.entries[0].key = key;

        leaf.entries[0].value = pointer;

        leaf.nextPageId = -1;

        if (!pageManager.writeLeafPage(
                pageId,
                leaf))
        {
            return false;
        }

        rootPageId = pageId;

        BPlusTreeMetadata metadata{};

        if (!pageManager.readMetadata(
                metadata))
        {
            return false;
        }

        metadata.rootPageId =
            rootPageId;

        metadata.pageCount =
            bufferPool->GetPageCount(
                pageManager.getFilename());

        return pageManager.writeMetadata(
            metadata);
    }

    // =========================================
    // CASE 2: Existing root
    // =========================================

    LeafPage leaf{};

    if (!pageManager.readLeafPage(
            rootPageId,
            leaf))
    {
        return false;
    }

    // =========================================
    // Duplicate check
    // =========================================

    for (int i = 0;
         i < leaf.header.size;
         i++)
    {
        if (leaf.entries[i].key == key)
        {
            std::cout
                << "Duplicate key\n";

            return false;
        }
    }

    // =========================================
    // Full leaf
    // =========================================

    if (leaf.header.size >=
        LEAF_MAX_ENTRIES)
    {
        LeafEntry temp[LEAF_MAX_ENTRIES + 1];

        int position = 0;

        while (
            position < leaf.header.size &&
            leaf.entries[position].key < key)
        {
            position++;
        }

        for (int i = 0;
             i < position;
             i++)
        {
            temp[i] =
                leaf.entries[i];
        }

        temp[position].key =
            key;

        temp[position].value =
            pointer;

        for (int i = position;
             i < leaf.header.size;
             i++)
        {
            temp[i + 1] =
                leaf.entries[i];
        }

        int newLeafPageId;

        int separatorKey;

        if (!splitLeafPage(
                rootPageId,
                temp,
                LEAF_MAX_ENTRIES + 1,
                newLeafPageId,
                separatorKey))
        {
            return false;
        }

        std::cout
            << "Leaf split successful\n";

        std::cout
            << "Left Page: "
            << rootPageId
            << '\n';

        std::cout
            << "Right Page: "
            << newLeafPageId
            << '\n';

        std::cout
            << "Separator: "
            << separatorKey
            << '\n';

        return true;
    }
    // =========================================
    // Find sorted insertion position
    // =========================================

    int position = 0;

    while (
        position < leaf.header.size &&
        leaf.entries[position].key < key)
    {
        position++;
    }

    // =========================================
    // Shift entries
    // =========================================

    for (int i = leaf.header.size;
         i > position;
         i--)
    {
        leaf.entries[i] =
            leaf.entries[i - 1];
    }

    // =========================================
    // Insert
    // =========================================

    leaf.entries[position].key =
        key;

    leaf.entries[position].value =
        pointer;

    leaf.header.size++;

    // =========================================
    // Write page
    // =========================================

    return pageManager.writeLeafPage(
        rootPageId,
        leaf);
}

RecordPointer BPlusTree::search(
    int key)
{
    if (empty())
        return RecordPointer();

    LeafPage leaf{};

    if (!pageManager.readLeafPage(
            rootPageId,
            leaf))
    {
        return RecordPointer();
    }

    for (int i = 0;
         i < leaf.header.size;
         i++)
    {
        if (leaf.entries[i].key == key)
        {
            return leaf.entries[i].value;
        }
    }

    return RecordPointer();
}

void BPlusTree::printRootLeaf() const
{
    if (rootPageId == -1)
    {
        std::cout
            << "Tree Empty\n";

        return;
    }

    LeafPage leaf{};

    BPlusTreePageManager &manager =
        const_cast<BPlusTreePageManager &>(
            pageManager);

    if (!manager.readLeafPage(
            rootPageId,
            leaf))
    {
        std::cout
            << "Unable to read root leaf\n";

        return;
    }

    std::cout
        << "[ ";

    for (int i = 0;
         i < leaf.header.size;
         i++)
    {
        std::cout
            << leaf.entries[i].key
            << " ";
    }

    std::cout
        << "]\n";
}

bool BPlusTree::splitLeafPage(
    int leafPageId,
    const LeafEntry *entries,
    int entryCount,
    int &newLeafPageId,
    int &separatorKey)
{
    if (entryCount !=
        LEAF_MAX_ENTRIES + 1)
    {
        return false;
    }

    LeafPage oldLeaf{};

    if (!pageManager.readLeafPage(
            leafPageId,
            oldLeaf))
    {
        return false;
    }

    newLeafPageId =
        pageManager.allocateLeafPage();

    if (newLeafPageId == -1)
        return false;

    LeafPage newLeaf{};

    if (!pageManager.readLeafPage(
            newLeafPageId,
            newLeaf))
    {
        return false;
    }

    int mid =
        entryCount / 2;

    // =====================================
    // Left
    // =====================================

    oldLeaf.header.size = mid;

    for (int i = 0;
         i < mid;
         i++)
    {
        oldLeaf.entries[i] =
            entries[i];
    }

    // =====================================
    // Right
    // =====================================

    newLeaf.header.size =
        entryCount - mid;

    for (int i = mid;
         i < entryCount;
         i++)
    {
        newLeaf.entries[i - mid] =
            entries[i];
    }

    // =====================================
    // Leaf chain
    // =====================================

    newLeaf.nextPageId =
        oldLeaf.nextPageId;

    oldLeaf.nextPageId =
        newLeafPageId;

    // =====================================
    // Separator
    // =====================================

    separatorKey =
        newLeaf.entries[0].key;

    // =====================================
    // Write
    // =====================================

    if (!pageManager.writeLeafPage(
            leafPageId,
            oldLeaf))
    {
        return false;
    }

    if (!pageManager.writeLeafPage(
            newLeafPageId,
            newLeaf))
    {
        return false;
    }

    return true;
}

bool BPlusTree::readLeafPage(
    int pageId,
    LeafPage &leaf)
{
    return pageManager.readLeafPage(
        pageId,
        leaf);
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

// bool BPlusTree::insertFirst(
//     int key,
//     const RecordPointer &pointer)
// {
//     if (!empty())
//     {
//         std::cout
//             << "Tree already has a root\n";

//         return false;
//     }

//     int pageId =
//         pageManager.allocateLeafPage();

//     if (pageId == -1)
//         return false;

//     LeafPage leaf{};

//     if (!pageManager.readLeafPage(
//             pageId,
//             leaf))
//     {
//         return false;
//     }

//     leaf.header.size = 1;

//     leaf.entries[0].key =
//         key;

//     leaf.entries[0].value =
//         pointer;

//     leaf.nextPageId = -1;

//     if (!pageManager.writeLeafPage(
//             pageId,
//             leaf))
//     {
//         return false;
//     }

//     rootPageId = pageId;

//     BPlusTreeMetadata metadata{};

//     if (!pageManager.readMetadata(
//             metadata))
//     {
//         return false;
//     }

//     metadata.rootPageId =
//         rootPageId;

//     metadata.pageCount =
//         bufferPool->GetPageCount(
//             pageManager.getFilename());

//     if (!pageManager.writeMetadata(
//             metadata))
//     {
//         return false;
//     }

//     return true;
// }

// void BPlusTree::printTree() const
// {
//     if (root == nullptr)
//     {
//         std::cout
//             << "Tree Empty\n";

//         return;
//     }

//     if (root->isLeaf)
//     {
//         LeafNode *leaf =
//             static_cast<LeafNode *>(root);

//         std::cout << "Leaf : ";

//         for (int key : leaf->keys)
//             std::cout << key << " ";

//         std::cout << '\n';

//         return;
//     }

//     InternalNode *internal =
//         static_cast<InternalNode *>(root);

//     std::cout
//         << "Root : ";

//     for (int key : internal->keys)
//     {
//         std::cout
//             << key
//             << " ";
//     }

//     std::cout << '\n';

//     for (Node *child :
//          internal->children)
//     {
//         LeafNode *leaf =
//             static_cast<LeafNode *>(child);

//         std::cout
//             << "[ ";

//         for (int key : leaf->keys)
//             std::cout
//                 << key
//                 << " ";

//         std::cout
//             << "] ";
//     }
//     std::cout << '\n';
// }

// LeafNode *BPlusTree::findLeaf(int key)
// {
//     if (root == nullptr)
//         return nullptr;

//     Node *current = root;

//     while (!current->isLeaf)
//     {
//         InternalNode *internal = static_cast<InternalNode *>(current);

//         int i = 0;

//         while (i < internal->keys.size() &&
//                key >= internal->keys[i])
//         {
//             i++;
//         }

//         current = internal->children[i];
//     }

//     return static_cast<LeafNode *>(current);
// }

// void BPlusTree::insert(int key, const RecordPointer &pointer)
// {
//     if (root == nullptr)
//     {
//         LeafNode *leaf = new LeafNode();

//         leaf->keys.push_back(key);

//         leaf->values.push_back(pointer);

//         root = leaf;

//         return;
//     }

//     LeafNode *leaf = findLeaf(key);
//     auto duplicate =
//         std::lower_bound(
//             leaf->keys.begin(),
//             leaf->keys.end(),
//             key);

//     if (duplicate != leaf->keys.end() &&
//         *duplicate == key)
//     {
//         throw std::runtime_error("Duplicate key");
//     }

//     auto keyPos = std::lower_bound(
//         leaf->keys.begin(),
//         leaf->keys.end(),
//         key);

//     int index = keyPos - leaf->keys.begin();

//     leaf->keys.insert(keyPos, key);

//     leaf->values.insert(
//         leaf->values.begin() + index,
//         pointer);

//     if (leaf->keys.size() > ORDER)
//     {
//         splitLeaf(leaf);
//     }
//     std::cout << "\nAfter inserting " << key << '\n';

//     std::cout
//         << "Tree Height = "
//         << height()
//         << '\n';
//     if (!verifyTree())
//     {
//         std::cout
//             << "B+ TREE CORRUPTED AFTER INSERTING "
//             << key
//             << '\n';

//         return;
//     }
// }

// void BPlusTree::printLeaves() const
// {
//     if (root == nullptr)
//         return;

//     Node *current = root;

//     while (!current->isLeaf)
//     {
//         current =
//             static_cast<InternalNode *>(current)
//                 ->children.front();
//     }

//     LeafNode *leaf =
//         static_cast<LeafNode *>(current);

//     while (leaf)
//     {
//         std::cout << "[ ";

//         for (size_t i = 0; i < leaf->keys.size(); i++)
//         {
//             std::cout
//                 << leaf->keys[i]
//                 << "("
//                 << leaf->values[i].pageId
//                 << ","
//                 << leaf->values[i].slotId
//                 << ") ";
//         }

//         std::cout << "] -> ";

//         leaf = leaf->next;
//     }

//     std::cout << "nullptr\n";
// }

// void BPlusTree::insertIntoParent(Node *left,
//                                  int key,
//                                  Node *right)
// {
//     if (left == root)
//     {
//         InternalNode *newRoot = new InternalNode();

//         newRoot->keys.push_back(key);

//         newRoot->children.push_back(left);
//         newRoot->children.push_back(right);

//         left->parent = newRoot;
//         right->parent = newRoot;

//         root = newRoot;

//         return;
//     }

//     InternalNode *parent =
//         static_cast<InternalNode *>(left->parent);

//     int childIndex = 0;

//     while (parent->children[childIndex] != left)
//     {
//         childIndex++;
//     }

//     parent->keys.insert(
//         parent->keys.begin() + childIndex,
//         key);

//     parent->children.insert(
//         parent->children.begin() + childIndex + 1,
//         right);

//     right->parent = parent;

//     if (parent->keys.size() > ORDER)
//     {
//         splitInternalNode(parent);
//     }
// }

// void BPlusTree::splitInternalNode(InternalNode *node)
// {
//     InternalNode *newInternal = new InternalNode();
//     int mid = node->keys.size() / 2;
//     int promotedKey = node->keys[mid];

//     newInternal->keys.assign(node->keys.begin() + mid + 1, node->keys.end());
//     node->keys.resize(mid);

//     newInternal->children.assign(
//         node->children.begin() + mid + 1,
//         node->children.end());

//     node->children.resize(mid + 1);
//     for (Node *child : newInternal->children)
//         child->parent = newInternal;

//     insertIntoParent(node, promotedKey, newInternal);
// }

// bool BPlusTree::verifyTree() const
// {
//     if (root == nullptr)
//         return true;

//     int leafDepth = -1;

//     bool structureOK =
//         verifyNode(
//             root,
//             LLONG_MIN,
//             true,
//             LLONG_MAX,
//             true,
//             0,
//             leafDepth);

//     if (!structureOK)
//         return false;

//     if (!verifyLeafChain())
//         return false;

//     return true;
// }

// bool BPlusTree::verifyNode(
//     Node *node,
//     long long minKey,
//     bool minInclusive,
//     long long maxKey,
//     bool maxInclusive,
//     int depth,
//     int &leafDepth) const
// {
//     if (node == nullptr)
//         return false;

//     // ========================================================
//     // LEAF
//     // ========================================================

//     if (node->isLeaf)
//     {
//         LeafNode *leaf =
//             static_cast<LeafNode *>(node);

//         // Keys and values must have same size
//         if (leaf->keys.size() !=
//             leaf->values.size())
//         {
//             std::cout
//                 << "ERROR: Leaf keys/values mismatch\n";

//             return false;
//         }

//         // Leaf keys must be sorted
//         for (size_t i = 1;
//              i < leaf->keys.size();
//              i++)
//         {
//             if (leaf->keys[i - 1] >=
//                 leaf->keys[i])
//             {
//                 std::cout
//                     << "ERROR: Leaf keys not strictly sorted\n";

//                 return false;
//             }
//         }

//         // All keys must be inside allowed range
//         for (int key : leaf->keys)
//         {
//             if (minInclusive)
//             {
//                 if (key < minKey)
//                     return false;
//             }
//             else
//             {
//                 if (key <= minKey)
//                     return false;
//             }

//             if (maxInclusive)
//             {
//                 if (key > maxKey)
//                     return false;
//             }
//             else
//             {
//                 if (key >= maxKey)
//                     return false;
//             }
//         }

//         // Every leaf must be at same depth
//         if (leafDepth == -1)
//         {
//             leafDepth = depth;
//         }
//         else if (leafDepth != depth)
//         {
//             std::cout
//                 << "ERROR: Leaves have different depths\n";

//             return false;
//         }

//         return true;
//     }

//     // ========================================================
//     // INTERNAL NODE
//     // ========================================================

//     InternalNode *internal =
//         static_cast<InternalNode *>(node);

//     // Internal node invariant
//     if (internal->children.size() !=
//         internal->keys.size() + 1)
//     {
//         std::cout
//             << "ERROR: Internal node children != keys + 1\n";

//         return false;
//     }

//     // Internal keys must be strictly sorted
//     for (size_t i = 1;
//          i < internal->keys.size();
//          i++)
//     {
//         if (internal->keys[i - 1] >=
//             internal->keys[i])
//         {
//             std::cout
//                 << "ERROR: Internal keys not sorted\n";

//             return false;
//         }
//     }

//     // Parent pointer check
//     for (Node *child :
//          internal->children)
//     {
//         if (child == nullptr)
//         {
//             std::cout
//                 << "ERROR: Null child\n";

//             return false;
//         }

//         if (child->parent != node)
//         {
//             std::cout
//                 << "ERROR: Incorrect parent pointer\n";

//             return false;
//         }
//     }

//     // ========================================================
//     // Recursively verify children
//     // ========================================================

//     for (size_t i = 0;
//          i < internal->children.size();
//          i++)
//     {
//         long long childMin = minKey;
//         long long childMax = maxKey;

//         bool childMinInclusive =
//             minInclusive;

//         bool childMaxInclusive =
//             maxInclusive;

//         if (i > 0)
//         {
//             childMin =
//                 internal->keys[i - 1];

//             childMinInclusive = true;
//         }

//         if (i < internal->keys.size())
//         {
//             childMax =
//                 internal->keys[i];

//             childMaxInclusive = false;
//         }

//         if (!verifyNode(
//                 internal->children[i],
//                 childMin,
//                 childMinInclusive,
//                 childMax,
//                 childMaxInclusive,
//                 depth + 1,
//                 leafDepth))
//         {
//             return false;
//         }
//     }

//     return true;
// }

// int BPlusTree::height() const
// {
//     if (root == nullptr)
//         return 0;

//     int height = 0;

//     Node *current = root;

//     while (current != nullptr)
//     {
//         height++;

//         if (current->isLeaf)
//             break;

//         InternalNode *internal =
//             static_cast<InternalNode *>(current);

//         current =
//             internal->children.front();
//     }

//     return height;
// }

// void BPlusTree::printRootInfo() const
// {
//     if (root == nullptr)
//     {
//         std::cout << "Root: NULL\n";
//         return;
//     }

//     std::cout
//         << "Root is "
//         << (root->isLeaf ? "Leaf" : "Internal")
//         << '\n';

//     if (!root->isLeaf)
//     {
//         InternalNode *internal =
//             static_cast<InternalNode *>(root);

//         std::cout
//             << "Root Keys: ";

//         for (int key : internal->keys)
//         {
//             std::cout
//                 << key
//                 << " ";
//         }

//         std::cout << '\n';

//         std::cout
//             << "Root Children: "
//             << internal->children.size()
//             << '\n';
//     }
// }

// bool BPlusTree::verifyParentPointers(Node *node) const
// {
//     if (node == nullptr ||
//         node->isLeaf)
//     {
//         return true;
//     }

//     InternalNode *internal =
//         static_cast<InternalNode *>(node);

//     for (Node *child : internal->children)
//     {
//         if (child->parent != node)
//             return false;

//         if (!verifyParentPointers(child))
//             return false;
//     }

//     return true;
// }

// bool BPlusTree::verifyInternalStructure(Node *node) const
// {
//     if (node == nullptr ||
//         node->isLeaf)
//     {
//         return true;
//     }

//     InternalNode *internal =
//         static_cast<InternalNode *>(node);

//     if (internal->children.size() !=
//         internal->keys.size() + 1)
//     {
//         return false;
//     }

//     for (Node *child : internal->children)
//     {
//         if (!verifyInternalStructure(child))
//             return false;
//     }

//     return true;
// }

// bool BPlusTree::verifyLeafChain() const
// {
//     if (root == nullptr)
//         return true;

//     Node *current = root;

//     while (!current->isLeaf)
//     {
//         InternalNode *internal =
//             static_cast<InternalNode *>(current);

//         current =
//             internal->children.front();
//     }

//     LeafNode *leaf =
//         static_cast<LeafNode *>(current);

//     bool firstKey = true;
//     int previousKey = 0;

//     while (leaf != nullptr)
//     {
//         for (int key : leaf->keys)
//         {
//             if (!firstKey &&
//                 key <= previousKey)
//             {
//                 std::cout
//                     << "ERROR: Leaf chain not sorted\n";

//                 return false;
//             }

//             previousKey = key;
//             firstKey = false;
//         }

//         leaf = leaf->next;
//     }

//     return true;
// }