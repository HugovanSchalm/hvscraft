#include "cglm/vec3.h"
#include <hvsengine/mesh.h>
#include <hvsengine/chunk.h>
#include <hvsengine/block.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <open-simplex-noise.h>

const vertexcollection_t BACKFACEVERTICES[] = {
        {
              .position = { 0.5f,  0.5f, -0.5f },
              .textureCoords = { 0.0f, 1.0f },
        },
        {
            .position = { -0.5f,  0.5f, -0.5f },
            .textureCoords = { 1.0f, 1.0f },
        },
        {
            .position = { 0.5f, -0.5f, -0.5f },
            .textureCoords = { 0.0f, 0.0f },
        },
        {
            .position = { -0.5f, -0.5f, -0.5f },
            .textureCoords = {1.0f, 0.0f}
        },
};

const vertexcollection_t FRONTFACEVERTICES[] = {
        // Vertex coords        Texture coords
    {
        .position = { -0.5f,  0.5f,  0.5f },
        .textureCoords = { 0.0f, 1.0f },
    },
    {
        .position = { 0.5f,  0.5f,  0.5f },
        .textureCoords = { 1.0f, 1.0f },
    },
    {
        .position = { -0.5f, -0.5f,  0.5f },
        .textureCoords = { 0.0f, 0.0f },
    },
    {
        .position = { 0.5f, -0.5f,  0.5f },
        .textureCoords = { 1.0f, 0.0f },
    },
};

const vertexcollection_t BOTTOMFACEVERTICES[] = {
        // Vertex coords        Texture coords
    {
        .position = { -0.5f, -0.5f,  0.5f },
        .textureCoords = { 0.0f, 1.0f },
    },
    {
        .position = { 0.5f, -0.5f,  0.5f },
        .textureCoords = { 1.0f, 1.0f },
    },
    {
        .position = { -0.5f, -0.5f, -0.5f },
        .textureCoords = { 0.0f, 0.0f },
    },
    {
        .position = { 0.5f, -0.5f, -0.5f },
        .textureCoords = { 1.0f, 0.0f },
    },
};
         

const vertexcollection_t TOPFACEVERTICES[] = {
        // Vertex coords        Texture coords
    {
        .position = { -0.5f,  0.5f, -0.5f },
        .textureCoords = { 0.0f, 1.0f },
    },
    {
        .position = { 0.5f,  0.5f, -0.5f },
        .textureCoords = { 1.0f, 1.0f },
    },
    {
        .position = { -0.5f,  0.5f,  0.5f },
        .textureCoords = { 0.0f, 0.0f },
    },
    {
        .position = { 0.5f,  0.5f,  0.5f },
        .textureCoords = { 1.0f, 0.0f },
    },
};

const vertexcollection_t LEFTFACEVERTICES[] = {
    {
        .position = { -0.5f,  0.5f, -0.5f },
        .textureCoords = { 0.0f, 1.0f },
    },
    {
        .position = { -0.5f,  0.5f,  0.5f },
        .textureCoords = { 1.0f, 1.0f },
    },
    {
        .position = { -0.5f, -0.5f, -0.5f },
        .textureCoords = { 0.0f, 0.0f },
    },
    {
        .position = { -0.5f, -0.5f,  0.5f },
        .textureCoords = { 1.0f, 0.0f },
    },
};

const vertexcollection_t RIGHTFACEVERTICES[] = {
    {
        .position = { 0.5f,  0.5f,  0.5f },
        .textureCoords = { 0.0f, 1.0f },
    },
    {
        .position = { 0.5f,  0.5f, -0.5f },
        .textureCoords = { 1.0f, 1.0f },
    },
    {
        .position = { 0.5f, -0.5f,  0.5f },
        .textureCoords = { 0.0f, 0.0f },
    },
    {
        .position = { 0.5f, -0.5f, -0.5f },
        .textureCoords = { 1.0f, 0.0f },
    },
};

uint32_t *getBlock(uint32_t x, uint32_t y, uint32_t z, chunk_t *chunk) {
    if (x >= CHUNKSIZE) return 0;
    if (y >= CHUNKSIZE) return 0;
    if (z >= CHUNKSIZE) return 0;
    return &chunk->blocks[x + y * CHUNKSIZE + z * CHUNKSIZE * CHUNKSIZE];
}

void generateMesh(chunk_t *chunk){
    vertexcollection_t *vertices = (vertexcollection_t *) malloc(CHUNKSIZE * CHUNKSIZE * CHUNKSIZE * 6 * 4 * sizeof(vertexcollection_t));
    if (vertices == NULL) {
        printf("ERROR: failed to allocate vertices\n");
        return;
    }
    uint32_t *indices = (uint32_t *) malloc(CHUNKSIZE * CHUNKSIZE * CHUNKSIZE * 6 * 6 * sizeof(uint32_t));
    if (indices == NULL) {
        printf("ERROR: failed to allocate indices\n");
        return;
    }
    size_t vertex_count = 0;
    size_t index_base  = 0;
    size_t index_count = 0;

    for (size_t z = 0; z < CHUNKSIZE; z++) {
        for (size_t y = 0; y < CHUNKSIZE; y++) {
            for (size_t x = 0; x < CHUNKSIZE; x++) {
                if (*getBlock(x, y, z, chunk) == SOLID) {
                    uint32_t *otherblock;
                    vec3 offset = {(float) x, (float) y, (float) z};

                    otherblock = getBlock(x - 1, y, z, chunk);
                    if (!otherblock || *otherblock == AIR) {
                        memcpy((void *) &vertices[vertex_count], (void *) &LEFTFACEVERTICES, sizeof(LEFTFACEVERTICES));
                        for (size_t i = 0; i < 4; i++){
                            vec3 *newpos = &vertices[vertex_count].position;
                            glm_vec3_add(vertices[vertex_count].position, offset, vertices[vertex_count].position);
                            vertex_count++;
                        }

                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 3;
                        indices[index_count++] = index_base + 1;
                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 2;
                        indices[index_count++] = index_base + 3;
                        index_base += 4;
                    }

                    otherblock = getBlock(x + 1, y, z, chunk);
                    if (!otherblock || *otherblock == AIR) {
                        memcpy((void *) &vertices[vertex_count], (void *) &RIGHTFACEVERTICES, sizeof(RIGHTFACEVERTICES));
                        for (size_t i = 0; i < 4; i++){
                            vec3 *newpos = &vertices[vertex_count].position;
                            glm_vec3_add(vertices[vertex_count].position, offset, vertices[vertex_count].position);
                            vertex_count++;
                        }

                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 3;
                        indices[index_count++] = index_base + 1;
                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 2;
                        indices[index_count++] = index_base + 3;
                        index_base += 4;
                    }

                    otherblock = getBlock(x, y - 1, z, chunk);
                    if (!otherblock || *otherblock == AIR) {
                        memcpy((void *) &vertices[vertex_count], (void *) &BOTTOMFACEVERTICES, sizeof(BOTTOMFACEVERTICES));
                        for (size_t i = 0; i < 4; i++){
                            vec3 *newpos = &vertices[vertex_count].position;
                            glm_vec3_add(vertices[vertex_count].position, offset, vertices[vertex_count].position);
                            vertex_count++;
                        }

                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 3;
                        indices[index_count++] = index_base + 1;
                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 2;
                        indices[index_count++] = index_base + 3;
                        index_base += 4;
                    }

                    otherblock = getBlock(x, y + 1, z, chunk);
                    if (!otherblock || *otherblock == AIR) {
                        memcpy((void *) &vertices[vertex_count], (void *) &TOPFACEVERTICES, sizeof(TOPFACEVERTICES));
                        for (size_t i = 0; i < 4; i++){
                            vec3 *newpos = &vertices[vertex_count].position;
                            glm_vec3_add(vertices[vertex_count].position, offset, vertices[vertex_count].position);
                            vertex_count++;
                        }

                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 3;
                        indices[index_count++] = index_base + 1;
                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 2;
                        indices[index_count++] = index_base + 3;
                        index_base += 4;
                    }

                    otherblock = getBlock(x, y, z - 1, chunk);
                    if (!otherblock || *otherblock == AIR) {
                        memcpy((void *) &vertices[vertex_count], (void *) &BACKFACEVERTICES, sizeof(BACKFACEVERTICES));
                        for (size_t i = 0; i < 4; i++){
                            vec3 *newpos = &vertices[vertex_count].position;
                            glm_vec3_add(vertices[vertex_count].position, offset, vertices[vertex_count].position);
                            vertex_count++;
                        }

                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 3;
                        indices[index_count++] = index_base + 1;
                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 2;
                        indices[index_count++] = index_base + 3;
                        index_base += 4;
                    }

                    otherblock = getBlock(x, y, z + 1, chunk);
                    if (!otherblock || *otherblock == AIR) {
                        memcpy((void *) &vertices[vertex_count], (void *) &FRONTFACEVERTICES, sizeof(FRONTFACEVERTICES));
                        for (size_t i = 0; i < 4; i++){
                            vec3 *newpos = &vertices[vertex_count].position;
                            glm_vec3_add(vertices[vertex_count].position, offset, vertices[vertex_count].position);
                            vertex_count++;
                        }

                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 3;
                        indices[index_count++] = index_base + 1;
                        indices[index_count++] = index_base;
                        indices[index_count++] = index_base + 2;
                        indices[index_count++] = index_base + 3;
                        index_base += 4;
                    }
                }
            }
        }
    }

    mesh_setup(vertices, vertex_count, indices, index_count, &chunk->mesh);

    free(vertices);
    free(indices);
}

void chunk_render(shader_t *shader, chunk_t *chunk) {
    mesh_render(shader, &chunk->mesh);
}

void chunk_setblock(uint32_t x, uint32_t y, uint32_t z, blocktype_t type, chunk_t *chunk) {
    *getBlock(x, y, z, chunk) = type;
    generateMesh(chunk);
}

void chunk_generate(chunk_t *chunk) {
    srand(time(NULL));
    struct osn_context *ctx;
    open_simplex_noise(rand(), &ctx);

    for (size_t z = 0; z < CHUNKSIZE; z++) {
        for (size_t y = 0; y < CHUNKSIZE; y++) {
            for (size_t x = 0; x < CHUNKSIZE; x++) {
                float noise = open_simplex_noise2(ctx, (double) x / 75.0, (double) z / 75.0);
                size_t height = (noise + 1.0f / 2.0f) * CHUNKSIZE;
                if (y <= height) {
                    *getBlock(x, y, z, chunk) = SOLID;
                } else {
                    *getBlock(x, y, z, chunk) = AIR;
                }
            }
        }
    }
    generateMesh(chunk);
}

