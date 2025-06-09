#include <hvsengine/mesh.h>
#include <hvsengine/shaders.h>

void mesh_setup(vertexcollection_t *vertices, uint32_t vertex_count, uint32_t *indices, uint32_t index_count, mesh_t *mesh) {
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);

    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(vertexcollection_t), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(float), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexcollection_t), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertexcollection_t), (void *) offsetof(vertexcollection_t, textureCoords));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    mesh->index_count = index_count;
    mesh->vertex_count = vertex_count;
}

void mesh_render(shader_t *shader, mesh_t *mesh) {
    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
