#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QApplication>
#include <QTextCodec>
#include <QTime>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>

#include <shader.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <mesh.h>
#include <model.h>

class Shader;
class Model;

class MyWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
public:
    MyWidget(QWidget *parent);
    ~MyWidget() override;
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void keyPressEvent(QKeyEvent *key) override;
//    void mousePressEvent(QMouseEvent *e) override;
//    void mouseReleaseEvent(QMouseEvent *e) override;
//    void timerEvent(QTimerEvent *e) override;
private:
    Shader *ourShader;
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    QTime time;
    Model *ourModel;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

};

#endif // MYWIDGET_H
