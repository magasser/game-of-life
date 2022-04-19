#ifndef GL_H
#define GL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "../visualizer.h"


struct gl_data {
    GLFWwindow* window;
    GLuint shader_prog_id;
    GLuint mvp_id;
    GLuint color_id;
    GLuint vertex_array_id;
    GLuint vertex_buffer;
};

typedef struct {
    void (**vtable)();
    game_t* game;

    size_t width;
    size_t height;
    
    /* Internal GL related data */
    struct gl_data* _data;
} gl_t;

gl_t*   gl_create(game_t* game, size_t width, size_t height);
void    free_gl(gl_t* gl);

#endif