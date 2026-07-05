#ifndef FRAME_H
#define FRAME_H

#include "../storage/page.h"

struct Frame
{
    // Actual page stored in RAM
    Page page;

    // Which page from disk is stored here
    int pageId;

    // Has this page been modified?
    bool dirty;

    // Number of users currently using this page
    int pinCount;

    // Is this frame currently occupied?
    bool isUsed;

    Frame()
    {
        pageId = -1;
        dirty = false;
        pinCount = 0;
        isUsed = false;
    }
};

#endif