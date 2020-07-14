#include "texturemesh.h"

TextureMesh::TextureMesh(vector<Point3d> point_vec, vector<unsigned int> indices)
{
    this->pointVec = point_vec;
    this->indices = indices;
    f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    initTextureDXF();
}

TextureMesh::~TextureMesh()
{

}

void TextureMesh::drawTextureDXF()
{
    f->glBindVertexArray(VAO);
    f->glDrawElements(GL_LINES, int(indices.size()), GL_UNSIGNED_INT, nullptr);
    f->glBindVertexArray(0);
}

void TextureMesh::initTextureDXF()
{
    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);
    f->glGenBuffers(1, &EBO);

    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, int(pointVec.size() * sizeof(Point3d)), &pointVec[0], GL_STATIC_DRAW);

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, int(indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Point3d), nullptr);

    f->glBindVertexArray(0);
}
