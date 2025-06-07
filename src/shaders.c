#include "hvsengine/shaders.h"
#include "cglm/types.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Reads a shader file into memory.
 * Mallocs so you should free the returned data after use.
 */
char *readShaderFile(const char* path) {
    FILE *shaderFile;
    if ((shaderFile = fopen(path, "r")) == NULL) {
        printf("ERROR Cannot open shader file\n");
        return NULL;
    }

    fseek(shaderFile, 0, SEEK_END);
    long fileSize = ftell(shaderFile);
    rewind(shaderFile);

    char *contents = malloc(fileSize + 1);
    fread(contents, fileSize, 1, shaderFile);
    fclose(shaderFile);
    contents[fileSize] = 0;

    return contents;
}

int shader_create(const char* vertexPath, const char* fragmentPath, shader_t* shader) {
    const char *vertexSource = readShaderFile(vertexPath);
    int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    int32_t success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR: Vertex shader compilation failed.\n%s\n", infoLog);
        return 1;
    }

    const char *fragmentSource = readShaderFile(fragmentPath);
    int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR: Fragment shader compilation failed.\n%s\n", infoLog);
        return 1;
    }

    shader->id = glCreateProgram();

    glAttachShader(shader->id, vertexShader);
    glAttachShader(shader->id, fragmentShader);
    glLinkProgram(shader->id);
    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shader->id, 512, NULL, infoLog);
        printf("ERROR: Shader linking failed.\n%s\n", infoLog);
        return 1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free((char *) vertexSource);
    free((char *) fragmentSource);

    return 0;
}

void shader_use(shader_t* shader) {
    glUseProgram(shader->id);
}

void shader_setVec3(shader_t* shader, const char *name, vec3 val) {
    int location = glGetUniformLocation(shader->id, name);
    glUniform3fv(location, 1, (GLfloat *) val);
}

void shader_setFloat(shader_t* shader, const char *name, float val) {
    int location = glGetUniformLocation(shader->id, name);
    glUniform1f(location, val);
}

void shader_setMat4(shader_t* shader, const char *name, mat4 val) {
    int location = glGetUniformLocation(shader->id, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, (float *) val);
}
