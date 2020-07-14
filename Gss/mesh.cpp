#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;

    f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    //initializeOpenGLFunctions();

    initMesh();
}

Mesh::~Mesh()
{
    //delete f;
}

void Mesh::initMesh()
{
    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);
    f->glGenBuffers(1, &EBO);

    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, int(vertices.size() * sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, int(indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    f->glBindVertexArray(0);

}

void Mesh::drawMesh(Shader *shader)
{
    shader->use();//此处没有任何用处
    f->glBindVertexArray(VAO);
    f->glDrawElements(GL_TRIANGLES, int(indices.size()), GL_UNSIGNED_INT, nullptr);
    f->glBindVertexArray(0);
}

void Mesh::drawMesh()
{
    f->glBindVertexArray(VAO);
    f->glDrawElements(GL_TRIANGLES, int(indices.size()), GL_UNSIGNED_INT, nullptr);
    f->glBindVertexArray(0);
}
