#include "mywidget.h"
#include <glm/gtx/string_cast.hpp>

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
    lightPos = glm::vec3(400.0f, 400.0f, 400.0f);
    lightswitch = false;
    Select_Mode = 2;
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
    ourShader->setVec3("light.position", lightPos);
    ourShader->setVec3("viewPos", cameraPos);

    ourShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    ourShader->setVec3("light.duffuse", 1.0f, 1.0f, 1.0f);
    ourShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    ourShader->setVec3("material.ambient", 0.05f, 0.05f, 0.05f);
    ourShader->setVec3("material.diffuse", 0.3f, 0.3f, 0.3f);
    ourShader->setVec3("material.specular", 0.7f, 0.7f, 0.7f);
    ourShader->setFloat("material.shininess", 2.0f);

    //模型缩放和移动位置计算
    ourModel->MiddlePos = (ourModel->MinPos + ourModel->MaxPos) / 2.0f;
    float width = 2 * cameraPos.z * glm::tan(float(45.0 * 0.5));
    float height = width * this->height() / this->width();
    float model_width = ourModel->MaxPos.x - ourModel->MinPos.x;
    float model_height = ourModel->MaxPos.y - ourModel->MinPos.y;
    float Max_model_length = glm::max(model_width, model_height);
    ourModel->MIN = glm::min(width / Max_model_length, height / Max_model_length);
    ourModel->MovePos = - ourModel->MiddlePos * ourModel->MIN;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    time.start();
}

void MyWidget::paintGL()
{
    //测试每帧渲染的速度
    float currentFrame = time.elapsed() / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //cout << "paintGL" << endl;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    //渲染模式选择 点模式，线模式，填充模式
    switch (Select_Mode) {
        case(1) :
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        }
        case(2) :
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        }
        case(3) :
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        }
    default:
        {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
        }
    }

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    ourShader->setMat4("view", view);

    float get_time = time.elapsed();//std::cout << get_time << std::endl;
    model = glm::mat4(1.0f);//std::cout<<glm::to_string(model)<<std::endl;
    model = glm::rotate(model, glm::radians(get_time/30), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, ourModel->MovePos);
    model = glm::scale(model, glm::vec3(ourModel->MIN));
    model = glm::scale(model, glm::vec3(0.8f));

    ourShader->setMat4("model", model);
    ourShader->setInt("lightswitch", lightswitch);

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
    if (key->key()==Qt::Key_Escape) QApplication::exit();
    float cameraSpeed = 2.5f * deltaTime;
    if (key->key()==Qt::Key_W) { cameraPos += cameraSpeed * cameraFront;}
    if (key->key()==Qt::Key_S){ cameraPos -= cameraSpeed * cameraFront;}
    if (key->key()==Qt::Key_A){ cameraPos.x = cameraPos.x + 0.05f;}
    if (key->key()==Qt::Key_D){ cameraPos.x = cameraPos.x - 0.05f;}
    if (key->key()==Qt::Key_L){ lightswitch = !lightswitch;}
    if (key->key()==Qt::Key_1){ Select_Mode = 1;}
    if (key->key()==Qt::Key_2){ Select_Mode = 2;}
    if (key->key()==Qt::Key_3){ Select_Mode = 3;}
    update();

}
