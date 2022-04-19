#include <string.h>

#include "reader_factory.h"

#include "text_reader.h"
#include "rle_reader.h"

reader_t* reader_create(const char* file_type) {
    reader_t* reader;
    if (strcmp(file_type, "txt") == 0) {
        reader = (reader_t*)txt_reader_create();
    } else if (strcmp(file_type, "rle") == 0) {
        reader = (reader_t*)rle_reader_create();
    }

    return reader;
}