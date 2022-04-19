#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../helpers.h"
#include "rle_reader.h"

#define CHAR_DEAD       'b'
#define CHAR_ALIVE      'o'
#define CHAR_EOL        '$'
#define CHAR_END        '!'
#define CHAR_COMMENT    '#'

#define MAX_LINE_LENGTH 100
#define MAX_NBR_LENGTH  10 

game_t* rle_reader_file_import(const rle_reader_t* reader, const char* file_name);
void    free_rle_reader(rle_reader_t* reader);

void (*rle_reader_vtable[])() = {
    [CALL_FREE] = free_rle_reader,
};

void* (*rle_reader_ptr_vtable[])() = {
    [CALL_IMPORT] = (void*)rle_reader_file_import,
};

rle_reader_t* rle_reader_create(void) {
    rle_reader_t* reader = malloc(sizeof(rle_reader_t));
    
    reader->vtable = rle_reader_vtable;
    reader->ptr_vtable = rle_reader_ptr_vtable;

    return reader;
}

game_t* rle_reader_file_import(const rle_reader_t* reader, const char* file_name) {
    FILE* file = fopen(file_name, "rb");    

    if (file == NULL) {
        printf_err("No such file or directory '%s'.", file_name);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_LINE_LENGTH];

    /* Skip comment lines */
    uint64_t file_index = 0;
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        file_index += strlen(buffer) + 1;
        if (buffer[0] != CHAR_COMMENT) {
            break;
        }

        memset(buffer, 0, MAX_LINE_LENGTH);
    }

    int32_t width, height, ph1, ph2;
    sscanf(buffer, "x = %d, y = %d, rule = B%d/S%d", &width, &height, &ph1, &ph2);
    file_index += strlen("x = %d, y = %d, rule = B%d/S%d");
    
    uint8_t* cells = calloc(width * height, sizeof(uint8_t));

    memset(buffer, 0, MAX_LINE_LENGTH);
    
    uint64_t cell_index = 0;
    char c = fgetc(file);
    while (c != CHAR_END) {
        if (c == '\n' || c == '\r' || c == CHAR_EOL) {
            c = fgetc(file);
            continue;
        }

        char number[MAX_NBR_LENGTH];
        uint8_t index = 0;
        while (isdigit(c)) {
            number[index] = c;
            ++index;
            c = fgetc(file);
        }

        uint8_t has_number = index > 0;

        if (has_number) {
            number[index] = '\0';

            uint8_t state = c == CHAR_ALIVE ? ALIVE : DEAD;
            uint16_t value;
            sscanf(number, "%d", &value);

            memset(cells + cell_index, state, value * sizeof(uint8_t));
            cell_index += value;
        } else {
            cells[cell_index] = c == CHAR_ALIVE ? ALIVE : DEAD;
            ++cell_index;
        }

        c = fgetc(file);
    }

    fclose(file);

    return game_create(cells, width, height);;
}

void free_rle_reader(rle_reader_t* reader) {
    free(reader);
}


