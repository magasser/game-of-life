#ifndef GL_H
#define GL_H

#include "visualizer.h"

typedef struct {
    void (**vtable)();
    game_t* game;
} gl_t;

gl_t*   gl_create(game_t* game);
void    free_gl(gl_t* gl);

#endif