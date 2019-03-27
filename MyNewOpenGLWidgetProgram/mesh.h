#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <QVector2D>
#include <QVector3D>
#include <vector>

using namespace std;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Mesh : protected QOpenGLFunctions_3_3_Core
{
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    //Mesh();
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
    virtual ~Mesh();

    void drawMesh(QOpenGLShaderProgram *program);

private:
    void initMesh();

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // MESH_H
