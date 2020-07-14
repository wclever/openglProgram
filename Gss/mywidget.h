#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QApplication>
#include <QTextCodec>
#include <QTime>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>
#include <QFileDialog>
#include <QAbstractButton>
#include <QtDebug>
#include <QVector2D>


#include <shader.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <mesh.h>
#include <model.h>
#include <axismesh.h>
#include <dxfreader.h>
#include <textureprofile.h>
#include <texturemesh.h>

class Shader;
class Model;
class Mesh;
class AxisMesh;
class DxfReader;
class TextureProfile;

class MyWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    MyWidget(QWidget *parent);
    ~MyWidget() override;
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void keyPressEvent(QKeyEvent *key) override;
    void wheelEvent(QWheelEvent * event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *e) override;
//    void timerEvent(QTimerEvent *e) override;
private:
    void loadModel(string filename);
    void shaderMode(int mode);
    void loadTexture(string dxfPath);
    void loadTexturePos(string texturePath);
    Shader *ourShader;
    Shader *normalShader;
    Shader *axisShader;
    Shader *textureShader;

    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 axismodel;
    glm::mat4 texturemodel;
    QTime time;
    QVector2D startPos;
    Model *ourModel;
    AxisMesh *ourAxis;
    DxfReader *ourDxf;
    TextureProfile *ourTexturePos;
    TextureMesh *ourTexture;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 lightPos;
    bool lightswitch;
    bool normalswitch;
    bool axisswitch;
    bool textureswitch;
    int Select_Mode;
    string filename;
    string dxfPath;
    string texturePath;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float rotateYValue = 0.0f;
    float startX = 0.0f;
    vector<Point3d> textureNormal;

public slots:
    void Open();
    void OpenTexture();
    void OpenTexturePos();
    void lighting_switch(int state);
    void normal_switch(int state);
    void axis_switch(int state);
    void texture_switch(int state);
    void point_mode(bool checked);
    void line_mode(bool checked);
    void fill_mode(bool checked);
};

#endif // MYWIDGET_H
