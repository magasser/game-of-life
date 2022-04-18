#include <stdlib.h>
#include <stdio.h>

#include "../../helpers.h"
#include "gl.h"
#include "shaders/shaders.h"

void gl_init(gl_t* gl);
void gl_update(gl_t* gl);

void (*gl_vtable[])() = {
    [CALL_INIT] = gl_init,
    [CALL_UPDATE] = gl_update,
};

static GLFWwindow* create_window(void);

void gl_init(gl_t* gl) {

    gl->window = create_window();
    
    glfwSetInputMode(gl->window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint vertex_array_id;
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    GLuint prog_id = load_shaders("src/ui/gl/shaders/SimpleVertexShader.vert", "src/ui/gl/shaders/SimpleFragmentShader.frag");

    static const GLfloat g_vertex_buffer_data[] = {
        -0.1f, -0.1f, 0.0f,
        0.1f, -0.1f, 0.0f,
        0.0f, 0.1f, 0.0f,
        //-0.1f, 0.1f, 0.0f,
    };

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(prog_id);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(gl->window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(gl->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(gl->window) == 0 );

        glDeleteBuffers(1, &vertex_buffer);
        glDeleteVertexArrays(1, &vertex_array_id);
        glDeleteProgram(prog_id);

        glfwTerminate();
}

void gl_update(gl_t* gl) {

}

gl_t* gl_create(game_t* game) {
    gl_t* gl = malloc(sizeof(gl_t));

    gl->vtable = gl_vtable;
    gl->game = game;
    gl->window = NULL;

    return gl;
}

void free_gl(gl_t* gl) {
    free(gl);
}

GLFWwindow* create_window(void) {
    glewExperimental = true;
    if (glfwInit() != GLFW_TRUE) {
        printf_err("Failed to initialize GLFW.");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "GL GameOfLIfe", NULL, NULL);

    if (window == NULL) {
        printf_err("Failed to open GLFW window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        printf_err("Failed to initialize GLEW.");
        exit(EXIT_FAILURE);
    }
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return window;
}
