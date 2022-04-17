#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gl.h"

void gl_init(gl_t* vis);
void gl_update(gl_t* vis);

void (*gl_vtable[])() = {
    [CALL_INIT] = gl_init,
    [CALL_UPDATE] = gl_update,
};

void gl_init(gl_t* vis) {

}

void gl_update(gl_t* vis) {

}
