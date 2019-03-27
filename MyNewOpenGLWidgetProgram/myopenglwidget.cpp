#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent):
    QOpenGLWidget (parent),
    m_program(this),
    meshes(nullptr),
    ourModel(nullptr),
    angularSpeed(0)
{
    cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
    cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
}


MyOpenGLWidget::~MyOpenGLWidget()
{
    makeCurrent();
    delete meshes;
    doneCurrent();
}


void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    initShaders();

    glEnable(GL_DEPTH_TEST);

    const char* path = "C:/Users/chwan/Documents/Qt/OriginalOpenGLProgram/model/cube.stl";
    string filename = string(QTextCodec::codecForLocale()->fromUnicode(path).data());
    ourModel = new Model(filename);

    timer.start(12, this);
}


void MyOpenGLWidget::initShaders()
{
        if(!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertexshader.vert"))
            close();

        if(!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragmentshader.frag"))
            close();

        if(!m_program.link())
            close();

        if(!m_program.bind())
            close();
}


void MyOpenGLWidget::paintGL()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    QMatrix4x4 model;
    model.translate(0.0, 0.0, -5.0);
    model.rotate(rotation);


    m_program.setUniformValue("mvp_matrix", projection * model);

    ourModel->Draw(&m_program);
    //update();
}


void MyOpenGLWidget::resizeGL(int w, int h)
{
    float aspect = float(w) / float(h ? h : 1);
    float zNear = 3.0, zFar = 7.0, fov = 45.0;
    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}


//鼠标操作
void MyOpenGLWidget::mousePressEvent(QMouseEvent *e)
{
    mousePressPosition = QVector2D(e->localPos());
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
    qreal acc = double(diff.length()) / 100.0;
    rotationAxis = (rotationAxis * float(angularSpeed) + n * float(acc)).normalized();
    angularSpeed += acc;
}

void MyOpenGLWidget::timerEvent(QTimerEvent *)
{
    angularSpeed *= 0.99;
    if (angularSpeed < 0.01)
        angularSpeed = 0.0;
    else {
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, float(angularSpeed)) * rotation;
        update();
    }
}
