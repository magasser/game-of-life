#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../../../helpers.h"
#include "shaders.h"

char* read_shader_code(const char* shader_file);

GLuint load_shaders(const char* vert_file_path, const char* frag_file_path) {
    GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);

    char* vert_code = read_shader_code(vert_file_path);
    char* frag_code = read_shader_code(frag_file_path);

    GLuint result = GL_FALSE;

    uint32_t info_log_length;

    const GLchar* vert_codes[] = {
        vert_code
    };
    glShaderSource(vert_id, 1, vert_codes, NULL);
    glCompileShader(vert_id);

    glGetShaderiv(vert_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vert_id, GL_INFO_LOG_LENGTH, &info_log_length);
    
    if (info_log_length > 0) {
        char* vert_error = malloc((info_log_length + 1) * sizeof(char));
        glGetShaderInfoLog(vert_id, info_log_length, NULL, vert_error);
        printf("%s\n", vert_error);
        free(vert_error);
    }

    const GLchar* frag_codes[] = {
        frag_code
    };
    glShaderSource(frag_id, 1, frag_codes, NULL);
    glCompileShader(frag_id);

    glGetShaderiv(frag_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(frag_id, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 0) {
        char* frag_error = malloc((info_log_length + 1) * sizeof(char));
        glGetShaderInfoLog(frag_id, info_log_length, NULL, frag_error);
        printf("%s\n", frag_error);
        free(frag_error);
    }

    GLuint prog_id = glCreateProgram();
    glAttachShader(prog_id, vert_id);
    glAttachShader(prog_id, frag_id);
    glLinkProgram(prog_id);
    glValidateProgram(prog_id);

    glGetProgramiv(prog_id, GL_LINK_STATUS, &result);
    glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 0) {
        char* prog_error = malloc((info_log_length + 1) * sizeof(char));
        glGetShaderInfoLog(prog_id, info_log_length, NULL, prog_error);
        printf("%s\n", prog_error);
        free(prog_error);
    }

    //glDetachShader(prog_id, vert_id);
    //glDetachShader(prog_id, frag_id);

    glDeleteShader(vert_id);
    glDeleteShader(frag_id);

    free(vert_code);
    free(frag_code);

    return prog_id;
}

char* read_shader_code(const char* shader_file) {

    FILE* file = fopen(shader_file, "rb");

    if (file == NULL) {
        printf_err("Failed to load shader from file '%s'.", shader_file);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    uint64_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* shader_code = malloc(file_size + 1);
    fread(shader_code, file_size, 1, file);
    fclose(file);

    shader_code[file_size] = '\0';

    return shader_code;
}