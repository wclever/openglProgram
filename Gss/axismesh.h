#ifndef AXISMESH_H
#define AXISMESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QOpenGLFunctions_3_3_Core>
#include <vector>
#include <QDebug>

using namespace std;

class AxisMesh
{
public:
    AxisMesh();
    virtual ~AxisMesh();
    const float vertices[72]={
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//x axis
        0.9f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.9f,-0.1f, 0.0f, 1.0f, 0.0f, 0.0f,//x arrow
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//y axis
       -0.1f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.1f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f,//y arrow
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,//z axis
       -0.1f, 0.0f, 0.9f, 0.0f, 0.0f, 1.0f,
        0.1f, 0.0f, 0.9f, 0.0f, 0.0f, 1.0f //z arrow
    };
    const unsigned int indices[18]={0, 1, 1, 2, 1, 3, 4, 5, 5, 6, 5, 7, 8, 9, 9, 10, 9, 11};
    unsigned int VAO;
    void drawMesh();
private:
    QOpenGLFunctions_3_3_Core *f;
    unsigned int VBO, EBO;
    void initMesh();

};

#endif // AXISMESH_H
