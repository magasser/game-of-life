#ifndef TEXT_READER_H
#define TEXT_READER_H

#include "file_reader.h"

typedef struct {
    void* (**ptr_vtable)();
    void (**vtable)();
} txt_reader_t;

txt_reader_t*   txt_reader_create(void);

#endif