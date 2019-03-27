#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QBasicTimer>
#include <QVector2D>
#include <QMouseEvent>
#include <QTextCodec>
#include <math.h>

#include <mesh.h>
#include <model.h>

class Mesh;
class Model;

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
public:
    MyOpenGLWidget(QWidget *parent);
    ~MyOpenGLWidget() override;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void initShaders();

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

private:
    QOpenGLShaderProgram m_program;
    QBasicTimer timer;
    QMatrix4x4 projection;

    QVector3D cameraPos;
    QVector3D cameraFront;
    QVector3D cameraUp;

    Mesh *meshes;
    Model *ourModel;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;

};

#endif // MYOPENGLWIDGET_H
