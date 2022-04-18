#ifndef GL_H
#define GL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "../visualizer.h"

typedef struct {
    void (**vtable)();
    game_t* game;

    GLFWwindow* window;
} gl_t;

gl_t*   gl_create(game_t* game);
void    free_gl(gl_t* gl);

#endif