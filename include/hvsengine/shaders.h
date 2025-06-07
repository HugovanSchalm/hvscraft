#include "cglm/types.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef _SHADERS_H
#define _SHADERS_H

typedef struct shader {
    int id;
} shader_t;

int shader_create(const char* vertexPath, const char* fragmentPath, shader_t* shader);

void shader_use(shader_t *shader);

void shader_setVec3(shader_t* shader, const char *name, vec3 val);

void shader_setFloat(shader_t* shader, const char *name, float val);

void shader_setMat4(shader_t* shader, const char *name, mat4 val);

#endif
