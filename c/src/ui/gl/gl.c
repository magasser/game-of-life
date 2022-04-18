#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../helpers.h"
#include "gl.h"
#include "shaders/shaders.h"

#define VERTEX_ATTRIB       0
#define COLOR_ATTRIB        1

#define VERTEX_SIZE         3
#define COLOR_SIZE          3

#define FLOATS_IN_CELL      18
#define TRIANGLES_IN_CELL   2

#define CELL_SIZE           200

void gl_init(gl_t* gl);
void gl_update(gl_t* gl);
void gl_terminate(gl_t* gl);

void (*gl_vtable[])() = {
    [CALL_INIT] = gl_init,
    [CALL_UPDATE] = gl_update,
    [CALL_TERMINATE] = gl_terminate,
};

static GLFWwindow* create_window(const gl_t* gl);
inline static void gl_update_buffers(gl_t* gl);
inline static void gl_update_window(struct gl_data data);
inline static void buffer_add_cell(const game_t* game, int32_t x, int32_t y, GLfloat* buffer, uint32_t index);


void gl_init(gl_t* gl) {
    struct gl_data* data = malloc(sizeof(struct gl_data));

    data->window = create_window(gl);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    data->shader_prog_id = load_shaders("src/ui/gl/shaders/SimpleVertexShader.vert", "src/ui/gl/shaders/SimpleFragmentShader.frag");

    glGenVertexArrays(1, &data->vertex_array_id);
    glBindVertexArray(data->vertex_array_id);

    data->mvp_id = glGetUniformLocation(data->shader_prog_id, "MVP");
    data->color_id = glGetUniformLocation(data->shader_prog_id, "Color");

    glGenBuffers(1, &data->vertex_buffer);

    gl->_data = data;
}

void gl_update(gl_t* gl) {
    if (glfwWindowShouldClose(gl->_data->window)) {
        gl_terminate(gl);
        return;
    }

    gl_update_buffers(gl);

    gl_update_window(*gl->_data);
}

void gl_terminate(gl_t* gl) {
        glDeleteBuffers(1, &gl->_data->vertex_buffer);
        glDeleteVertexArrays(1, &gl->_data->vertex_array_id);
        glDeleteProgram(gl->_data->shader_prog_id);

        glfwTerminate();
}

gl_t* gl_create(game_t* game, size_t width, size_t height) {
    gl_t* gl = malloc(sizeof(gl_t));

    gl->vtable = gl_vtable;
    gl->game = game;
    gl->height = height;
    gl->width = width;
    gl->_data = NULL;

    return gl;
}

void free_gl(gl_t* gl) {
    free(gl->_data->window);
    free(gl->_data);
    free(gl);
}

static GLFWwindow* create_window(const gl_t* gl) {
    glewExperimental = true;
    if (glfwInit() != GLFW_TRUE) {
        printf_err("Failed to initialize GLFW.");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(gl->width, gl->height, "GL GameOfLIfe", NULL, NULL);

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
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return window;
}

inline static void gl_update_buffers(gl_t* gl) {
    size_t buffer_size = gl->game->gen.alive * FLOATS_IN_CELL;
    GLfloat* vertex_buffer_data = malloc(buffer_size * sizeof(GLfloat));

    size_t length = gl->game->width * gl->game->height;
    uint32_t index = 0;
    for (uint64_t i = 0; i < length; ++i) {
        if (gl->game->cells[i] == ALIVE) {
            int32_t x = i % gl->game->width;
            int32_t y = i / gl->game->width;

            buffer_add_cell(gl->game, x, y, vertex_buffer_data, index);
            index += FLOATS_IN_CELL;
        }
    }
    
    mat4 projection;
    float half_width = gl->width / 2.0f;
    float half_height = gl->height / 2.0f;
    glm_ortho(-half_width, half_width, -half_height, half_height, 0.1f, 100.0f, projection);

    mat4 view;
    glm_lookat(
        (vec3){ 0, 0, 3 },
        (vec3){ 0, 0, 0 },
        (vec3){ 0, 1, 0 },
        view);

    vec3 scale = { CELL_SIZE, CELL_SIZE, 1 };

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_scale(model, scale);

    mat4 mvp;
    glm_mat4_mulN((mat4* []){ &projection, &view, &model }, 3, mvp);

    gl->_data->mvp = &mvp[0][0];

    glBindBuffer(GL_ARRAY_BUFFER, gl->_data->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, vertex_buffer_data, GL_STATIC_DRAW);

    gl->_data->nr_triangles = gl->game->gen.alive * TRIANGLES_IN_CELL;

    free(vertex_buffer_data);
}

inline static void gl_update_window(struct gl_data data) {    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(data.shader_prog_id);

        glUniformMatrix4fv(data.mvp_id, 1, GL_FALSE, data.mvp);
        glUniform3f(data.color_id, 0.9f, 0.9f, 0.9f);

        glEnableVertexAttribArray(VERTEX_ATTRIB);
        glBindBuffer(GL_ARRAY_BUFFER, data.vertex_buffer);
        glVertexAttribPointer(VERTEX_ATTRIB, VERTEX_SIZE, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, data.nr_triangles);
        glDisableVertexAttribArray(VERTEX_ATTRIB);

        glfwSwapBuffers(data.window);
        glfwPollEvents();
}

inline static void buffer_add_cell(const game_t* game, int32_t x, int32_t y, GLfloat* buffer, uint32_t index) {
    const float c_x = x - game->width / 4.0f;
    const float c_y = y + game->height / 4.0f;
    /*const float l = c_x - 0.5f;
    const float b = c_y - 0.5f;
    const float r = c_x + 0.5f;
    const float t = c_y + 0.5f;*/
    const float l = (c_x - 0.5f) / game->width;
    const float b = (c_y - 0.5f) / game->height;
    const float r = (c_x + 0.5f) / game->width;
    const float t = (c_y + 0.5f) / game->height;

    const GLfloat verticies[] = { 
        l, b, 0.0f,
        r, b, 0.0f,
        r, t, 0.0f,
        
        r, t, 0.0f,
        l, b, 0.0f,
        l, t, 0.0f,
    };

    memcpy(buffer + index, verticies, sizeof(verticies));
}
