#include "axismesh.h"

AxisMesh::AxisMesh()
{
    f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
//    qDebug()<<&vertices<<" "<<sizeof &vertices<<endl;
//    qDebug()<<vertices<<" "<<sizeof vertices<<endl;
//    qDebug()<<&vertices[0]<<" "<<sizeof &vertices[0]<<endl;
    initMesh();
}

AxisMesh::~AxisMesh()
{

}

void AxisMesh::drawMesh()
{
    f->glBindVertexArray(VAO);
    //qDebug()<<sizeof(indices)<<endl;
    f->glDrawElements(GL_LINES, sizeof(indices), GL_UNSIGNED_INT, nullptr);
    f->glBindVertexArray(0);
}

void AxisMesh::initMesh()
{
    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);
    f->glGenBuffers(1, &EBO);

    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);

    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    f->glBindVertexArray(0);
}
