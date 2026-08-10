#ifndef RECORD_POINTER_H
#define RECORD_POINTER_H

struct RecordPointer
{
    int pageId;

    int slotId;

    RecordPointer() : pageId(-1), slotId(-1) {}

    RecordPointer(int p, int s) : pageId(p), slotId(s) {}
};

#endif