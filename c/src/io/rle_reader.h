#ifndef RLE_READER_H
#define RLE_READER_H

#include "file_reader.h"

typedef struct {
    void* (**ptr_vtable)();
    void (**vtable)();
} rle_reader_t;

rle_reader_t*   rle_reader_create(void);

#endif