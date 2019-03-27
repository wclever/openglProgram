#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QOpenGLFunctions_3_3_Core>
#include <vector>

#include <shader.h>

using namespace std;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Mesh
{
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    unsigned int VAO;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
    virtual ~Mesh();

    void drawMesh(Shader* shader);
    void drawMesh();

private:
    QOpenGLFunctions_3_3_Core *f;
    unsigned int VBO, EBO;
    void initMesh();
};

#endif // MESH_H
