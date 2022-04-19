#include <stdlib.h>
#include <stdio.h>

#include "../helpers.h"
#include "text_reader.h"

#define CHAR_DEAD       '.'
#define CHAR_ALIVE      'X'

game_t* txt_reader_file_import(const txt_reader_t* reader, const char* file_name);
void    free_txt_reader(txt_reader_t* reader);

void (*txt_reader_vtable[])() = {
    [CALL_FREE] = free_txt_reader,    
};

void* (*txt_reader_ptr_vtable[])() = {
    [CALL_IMPORT] = (void*)txt_reader_file_import,
};


txt_reader_t* txt_reader_create(void) {
    txt_reader_t* reader = malloc(sizeof(txt_reader_t));
    
    reader->vtable = txt_reader_vtable;
    reader->ptr_vtable = txt_reader_ptr_vtable;

    return reader;
}

game_t* txt_reader_file_import(const txt_reader_t* reader, const char* file_name) {
    FILE* file = fopen(file_name, "rb");

    if (file == NULL) {
        printf_err("No such file or directory '%s'.", file_name);
        exit(EXIT_FAILURE);
    }

    char* lines = calloc(MAX_DIM_SIZE * MAX_DIM_SIZE, sizeof(char));

    uint16_t i = 0, k = 0, width = 0, height = 0;
    char current = fgetc(file);

    if (current == EOF) {
        printf_err("Incorrect file format in file '%s'.", file_name);
        exit(EXIT_FAILURE);
    }

    while (current != EOF) {
        if (current == '\n') {
            ++k;
            width = i;
            i = 0;
            current = fgetc(file);
            continue;
        } else if (current == '\r') {
            current = fgetc(file);
            continue;
        }

        if (current != CHAR_ALIVE && current != CHAR_DEAD) {
            printf_err("Incorrect file format in file '%s'.", file_name);
            exit(EXIT_FAILURE);
        }

        lines[k * MAX_DIM_SIZE + i] = current;

        ++i;
        current = fgetc(file);
    }

    height = k + 1;

    uint64_t length = width * height;
    uint8_t* cells = calloc(length, sizeof(uint8_t));

    for (uint16_t y = 0; y < height; ++y) {
        for (uint16_t x = 0; x < width; ++x) {
            cells[y * width + x] = lines[y * MAX_DIM_SIZE + x] == CHAR_ALIVE ? ALIVE : DEAD;
        }
    }

    fclose(file);

    return game_create(cells, width, height);
}

void free_txt_reader(txt_reader_t* reader) {
    free(reader);
}