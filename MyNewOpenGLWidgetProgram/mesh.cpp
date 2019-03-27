#include "mesh.h"


//Mesh::Mesh() : indexBuf(QOpenGLBuffer::IndexBuffer)
//{
//    initializeOpenGLFunctions();

//    arrayBuf.create();
//    indexBuf.create();

//    initMesh();
//}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices) : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    this->vertices = vertices;
    this->indices = indices;

    initializeOpenGLFunctions();

    arrayBuf.create();
    indexBuf.create();

    initMesh();
}

Mesh::~Mesh()
{
//    arrayBuf.destroy();
//    indexBuf.destroy();
}



void Mesh::initMesh()
{
//    GLfloat verts[] =
//    {
//        // Vertex data for face 0
//        -1.0f, -1.0f,  1.0f,  // v0
//         1.0f, -1.0f,  1.0f,  // v1
//        -1.0f,  1.0f,  1.0f,  // v2
//         1.0f,  1.0f,  1.0f,  // v3

//        // Vertex data for face 1
//         1.0f, -1.0f,  1.0f,  // v4
//         1.0f, -1.0f, -1.0f,  // v5
//         1.0f,  1.0f,  1.0f,  // v6
//         1.0f,  1.0f, -1.0f,  // v7

//        // Vertex data for face 2
//         1.0f, -1.0f, -1.0f,  // v8
//        -1.0f, -1.0f, -1.0f,  // v9
//         1.0f,  1.0f, -1.0f,  // v10
//        -1.0f,  1.0f, -1.0f,  // v11

//        // Vertex data for face 3
//        -1.0f, -1.0f, -1.0f,  // v12
//        -1.0f, -1.0f,  1.0f,  // v13
//        -1.0f,  1.0f, -1.0f,  // v14
//        -1.0f,  1.0f,  1.0f,  // v15

//        // Vertex data for face 4
//        -1.0f, -1.0f, -1.0f,  // v16
//         1.0f, -1.0f, -1.0f,  // v17
//        -1.0f, -1.0f,  1.0f,  // v18
//         1.0f, -1.0f,  1.0f,  // v19

//        // Vertex data for face 5
//        -1.0f,  1.0f,  1.0f,  // v20
//         1.0f,  1.0f,  1.0f,  // v21
//        -1.0f,  1.0f, -1.0f,  // v22
//         1.0f,  1.0f, -1.0f   // v23
//    };
//    GLushort indices[] = {
//         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
//         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
//         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
//        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
//        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
//        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
//    };


    arrayBuf.bind();
    indexBuf.bind();

//    arrayBuf.allocate(verts, sizeof (verts));
//    indexBuf.allocate(indices, sizeof (indices));
    arrayBuf.allocate(&vertices[0], int(vertices.size() * sizeof (Vertex)));
    indexBuf.allocate(&indices[0], int(indices.size() * sizeof (unsigned int)));
}

void Mesh::drawMesh(QOpenGLShaderProgram *program)
{
    arrayBuf.bind();
    indexBuf.bind();

    program->enableAttributeArray(0);
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof (Vertex));

    //glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
    glDrawElements(GL_TRIANGLES, int(indices.size()), GL_UNSIGNED_INT, nullptr);
}
