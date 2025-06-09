#ifndef _MESH_H
#define _MESH_H

#include "cglm/types.h"
#include "shaders.h"
#include <stdint.h>

typedef struct vertexCollection {
    vec3 position;
    vec2 textureCoords;
} vertexcollection_t;

typedef struct mesh {
    uint32_t vertex_count;
    uint32_t index_count;
    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;
} mesh_t;

void mesh_setup(vertexcollection_t *vertices, uint32_t vertex_count, uint32_t *indices, uint32_t index_count, mesh_t *mesh);

void mesh_render(shader_t *shader, mesh_t *mesh);

#endif // !_MESH_H
