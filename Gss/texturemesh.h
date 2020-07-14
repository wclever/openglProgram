#ifndef TEXTUREMESH_H
#define TEXTUREMESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QOpenGLFunctions_3_3_Core>
#include <vector>
#include <QDebug>
#include <datastruct.h>

using namespace std;

class TextureMesh
{
public:
    TextureMesh(vector<Point3d> point_vec, vector<unsigned int> indices);
    virtual ~TextureMesh();
    unsigned int VAO;
    void drawTextureDXF();
private:
    QOpenGLFunctions_3_3_Core *f;
    unsigned int VBO, EBO;
    void initTextureDXF();

    vector<Point3d> pointVec;
//    Line3dVec lineVec;
//    CircleArc3dVec circleArcVec;
    vector<unsigned int> indices;
//    vector<Point3d> circle;
//    vector<Point3d> arc;

};

#endif // TEXTUREMESH_H
