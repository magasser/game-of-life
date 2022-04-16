#ifndef CONSOLE_H
#define CONSOLE_H

#include "visualizer.h"

typedef struct {
    void (**vtable)();
    game_t* game;
} console_t;

console_t*  console_create(const game_t* game);
void        free_console(console_t* console);

#endif