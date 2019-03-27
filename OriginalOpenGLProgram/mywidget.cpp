#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent):
    QOpenGLWidget (parent),
    ourShader(nullptr),
    ourModel(nullptr)
{
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    setFocus();
}

MyWidget::~MyWidget()
{
    makeCurrent();
    delete ourShader;
    doneCurrent();
}

void MyWidget::initializeGL()
{
    cout << "initializeGL" << endl;
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    const char* path = "C:/Users/chwan/Documents/Qt/OriginalOpenGLProgram/model/炮弹.dxf";
    string filename = string(QTextCodec::codecForLocale()->fromUnicode(path).data());
    ourModel = new Model(filename);

    const char *vertexPath = "C:/Users/chwan/Documents/Qt/OriginalOpenGLProgram/vertexshader.vert";
    const char *fragmentPath = "C:/Users/chwan/Documents/Qt/OriginalOpenGLProgram/fragmentshader.frag";
    ourShader = new Shader(vertexPath, fragmentPath);
    ourShader->use();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    time.start();
}

void MyWidget::paintGL()
{

    float currentFrame = time.elapsed() / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //cout << "paintGL" << endl;
    //initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


    float get_time = time.elapsed();
    //std::cout << get_time << std::endl;
    model = glm::mat4(1.0f);

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    ourShader->setMat4("view", view);

    model = glm::translate(model, ourModel->MovePos);
    model = glm::rotate(model, glm::radians(get_time/30), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(ourModel->MIN));
    ourShader->setMat4("model", model);

    ourModel->Draw();

    update();
}

void MyWidget::resizeGL(int w, int h)
{
    initializeOpenGLFunctions();
    cout << "resizeGL" << endl;
    float aspect = float(w) / float(h ? h : 1);
    const float zNear = 0.1f, zFar = 100.0f, fov = 45.0f;

    projection = glm::perspective(glm::radians(fov), float(aspect), zNear, zFar);
    ourShader->setMat4("projection", projection);
}

void MyWidget::keyPressEvent(QKeyEvent *key)
{
    if (key->key()==Qt::Key_Escape)
        //close();
        QApplication::exit();
    //cout << "deltaTime" << deltaTime << endl;
    float cameraSpeed = 2.5f * deltaTime;
    if (key->key()==Qt::Key_W)
    {
        //cout << "press" << endl;
        cameraPos += cameraSpeed * cameraFront;
        update();
    }
    if (key->key()==Qt::Key_S)
    {
        //cout << "press" << endl;
        cameraPos -= cameraSpeed * cameraFront;
        update();
    }
//    if (key->key()==Qt::Key_1)
//    {
//        //cout << "press" << endl;
//        setFocus();
//        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  //GL_POINT GL_LINE GL_FILL
//        glPointSize(2.0f);
//        update();
//    }
//    if (key->key()==Qt::Key_2)
//    {
//        //cout << "press" << endl;
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //GL_POINT GL_LINE GL_FILL
//        //glLineWidth(2.0f);
//        update();
//    }
//    if (key->key()==Qt::Key_3)
//    {
//        //cout << "press" << endl;
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //GL_POINT GL_LINE GL_FILL
//        update();
//    }
}
