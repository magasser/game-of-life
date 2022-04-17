#include "file_reader.h"

game_t* file_import(const reader_t* reader, const char* file_name) {
    game_t* game = (game_t*)reader->ptr_vtable[CALL_IMPORT](reader, file_name);
    return game;
}

void free_reader(reader_t* reader) {
    reader->vtable[CALL_FREE](reader);
}