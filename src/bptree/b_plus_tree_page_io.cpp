#include "b_plus_tree_page_io.h"

#include <cstring>

// ============================================
// Leaf initialization
// ============================================

void initializeLeafPage(
    LeafPage& page,
    int32_t pageId)
{
    initializeBPlusPage(
        page.header,
        BPlusPageType::LEAF,
        pageId);

    page.nextPageId = -1;
}


// ============================================
// Internal initialization
// ============================================

void initializeInternalPage(
    InternalPage& page,
    int32_t pageId)
{
    initializeBPlusPage(
        page.header,
        BPlusPageType::INTERNAL,
        pageId);

    page.firstChildPageId = -1;
}


// ============================================
// Leaf serialization
// ============================================

void serializeLeafPage(
    const LeafPage& source,
    Page& destination)
{
    static_assert(
        sizeof(LeafPage) <= PAGE_SIZE,
        "LeafPage is too large");

    std::memset(
        destination.data,
        0,
        PAGE_SIZE);

    std::memcpy(
        destination.data,
        &source,
        sizeof(LeafPage));
}


// ============================================
// Leaf deserialization
// ============================================

void deserializeLeafPage(
    const Page& source,
    LeafPage& destination)
{
    std::memcpy(
        &destination,
        source.data,
        sizeof(LeafPage));
}


// ============================================
// Internal serialization
// ============================================

void serializeInternalPage(
    const InternalPage& source,
    Page& destination)
{
    static_assert(
        sizeof(InternalPage) <= PAGE_SIZE,
        "InternalPage is too large");

    std::memset(
        destination.data,
        0,
        PAGE_SIZE);

    std::memcpy(
        destination.data,
        &source,
        sizeof(InternalPage));
}


// ============================================
// Internal deserialization
// ============================================

void deserializeInternalPage(
    const Page& source,
    InternalPage& destination)
{
    std::memcpy(
        &destination,
        source.data,
        sizeof(InternalPage));
}