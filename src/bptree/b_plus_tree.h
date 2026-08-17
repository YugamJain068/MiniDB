#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include <string>
#include <vector>

#include "../buffer/buffer_pool_manager.h"
#include "../storage/record_pointer.h"

#include "b_plus_tree_page_manager.h"
#include "b_plus_tree_leaf_page.h"
#include "b_plus_tree_internal_page.h"

class BPlusTree
{
private:
    BufferPoolManager *bufferPool;

    BPlusTreePageManager pageManager;

    int rootPageId;

    bool splitLeafPage(
        int leafPageId,
        const LeafEntry *entries,
        int entryCount,
        int &newLeafPageId,
        int &separatorKey);

    bool createNewRoot(
        int leftPageId,
        int rightPageId,
        int separatorKey);

    bool insertIntoInternalPage(
        int parentPageId,
        int separatorKey,
        int rightChildPageId);

public:
    BPlusTree(
        BufferPoolManager *bpm,
        const std::string &filename);

    // ================================
    // Tree lifecycle
    // ================================

    bool initialize();

    bool empty() const;

    int getRootPageId() const;

    // ================================
    // Basic operations
    // ================================

    bool insert(
        int key,
        const RecordPointer &pointer);

    RecordPointer search(
        int key);

    // ================================
    // Day 9 testing
    // ================================

    bool readRootLeaf(
        LeafPage &leaf);

        void printRootLeaf() const;

    bool readLeafPage(
        int pageId,
        LeafPage &leaf);

    bool readInternalPage(
        int pageId,
        InternalPage &internal);

    int findLeafPage(
        int key);

    // ================================
    // Debug / validation
    // ================================

    // void printTree() const;

    // void printRootInfo() const;

    // int height() const;

    // bool verifyTree() const;
};

#endif