#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>

#include "../game/game.h"

typedef struct {
    void* (**ptr_vtable)();
    void (**vtable)();
} reader_t;

enum {
    CALL_IMPORT,
    CALL_FREE
};

game_t* file_import(const reader_t* reader, const char* file_name);

void    free_reader(reader_t* reader);

#endif