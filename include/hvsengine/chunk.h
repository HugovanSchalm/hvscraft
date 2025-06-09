#ifndef _CHUNKS_H
#define _CHUNKS_H

#include <stdint.h>
#include "block.h"
#include "hvsengine/shaders.h"
#include "mesh.h"

#define CHUNKSIZE 16

typedef struct vertex {
    float position[3];
    float textureCoords[3];
} vertex_t;

typedef struct chunk {
    mesh_t mesh;
    uint32_t blocks[CHUNKSIZE * CHUNKSIZE * CHUNKSIZE];
} chunk_t;

void chunk_generate(chunk_t *chunk);
void chunk_setblock(uint32_t x, uint32_t y, uint32_t z, blocktype_t type, chunk_t *chunk);
blocktype_t chunk_getblock(int32_t x, int32_t y, int32_t z, chunk_t *chunk);
void chunk_render(shader_t *shader, chunk_t *chunk);

#endif 
