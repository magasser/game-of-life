#ifndef READER_FACTORY_H
#define READER_FACTORY_H

#include "file_reader.h"

reader_t* reader_create(const char* file_type);

#endif