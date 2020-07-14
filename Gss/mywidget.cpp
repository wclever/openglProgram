#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent):
    QOpenGLWidget (parent),
    ourShader(nullptr),
    normalShader(nullptr),
    axisShader(nullptr),
    textureShader(nullptr),
    ourModel(nullptr),
    ourAxis(nullptr),
    ourDxf(nullptr),
    ourTexturePos(nullptr),
    ourTexture(nullptr)
{
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    axismodel = glm::mat4(1.0f);

    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    lightPos = glm::vec3(400.0f, 400.0f, 400.0f);
    lightswitch = false;
    normalswitch = false;
    axisswitch = false;
    textureswitch = false;
    Select_Mode = 3;
    setFocus();
}

MyWidget::~MyWidget()
{
    makeCurrent();
    delete ourShader;
    delete normalShader;
    delete ourAxis;
    delete textureShader;
    doneCurrent();
}

void MyWidget::initializeGL()
{
    //qDebug() << "initializeGL";
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    filename = "";
}

void MyWidget::paintGL()
{
    //qDebug() << "paintGL";

    //判断是否加载模型
    if(!filename.empty())
    {
        loadModel(filename);
        filename = "";
    }

    if(!dxfPath.empty())
    {
        loadTexture(dxfPath);
        dxfPath = "";
    }

    if(!texturePath.empty())
    {
        loadTexturePos(texturePath);
        texturePath = "";
    }

    //背景颜色填充
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    //模型渲染
    if(ourModel != nullptr)
    {
        //qDebug() << "模型渲染";
        //测试每帧渲染的速度
        float currentFrame = time.elapsed() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float MAGNITUDE = 0.05f * 0.7f * ourModel->MIN;

        //渲染模式选择 点模式，线模式，填充模式
        shaderMode(Select_Mode);

        //模型变换
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        //float get_time = time.elapsed();//std::cout << get_time << std::endl;
        model = glm::mat4(1.0f);//std::cout<<glm::to_string(model)<<std::endl;
        model = glm::rotate(model, glm::radians(rotateYValue), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(get_time/30), glm::vec3(0.0f, 1.0f, 0.0f));

        model = glm::scale(model, glm::vec3(ourModel->MIN * 0.6f));//qDebug() << "MIN: " << ourModel->MIN;
        model = glm::translate(model, ourModel->MovePos);

        float aspect = float(this->width()) / float(this->height() ? this->height() : 1);
        projection = glm::perspective(glm::radians(45.0f), aspect, 0.01f, 100.0f);

        //模型显示
        ourShader->use();
        ourShader->setMat4("projection", projection);
        ourShader->setMat4("view", view);
        ourShader->setMat4("model", model);
        ourShader->setInt("lightswitch", lightswitch);
        ourShader->setBool("normal_inverse_flag", ourModel->flag);//qDebug() << ourModel->flag;
        ourModel->Draw(ourShader);

        //法向量显示
        if(normalswitch)
        {
            normalShader->use();
            normalShader->setMat4("projection", projection);
            normalShader->setMat4("view", view);
            normalShader->setMat4("model", model);
            normalShader->setFloat("MAGNITUDE", MAGNITUDE);
            normalShader->setBool("normal_inverse_flag", ourModel->flag);
            ourModel->Draw(normalShader);
        }

        //坐标系显示
        if(axisswitch)
        {
            axismodel = glm::mat4(1.0f);
            axismodel = glm::rotate(axismodel, glm::radians(rotateYValue), glm::vec3(0.0f, 1.0f, 0.0f));
            //axismodel = glm::rotate(axismodel, glm::radians(get_time/30), glm::vec3(0.0f, 1.0f, 0.0f));
            axismodel = glm::scale(axismodel,glm::vec3(0.9f));
            axisShader->use();
            axisShader->setMat4("projection", projection);
            axisShader->setMat4("view", view);
            axisShader->setMat4("axismodel", axismodel);
            ourAxis->drawMesh();
        }

        //显示贴图单个模型
//        texturemodel = glm::mat4(1.0f);
//        texturemodel = glm::rotate(texturemodel, glm::radians(rotateYValue), glm::vec3(0.0f, 1.0f, 0.0f));
//        //texturemodel = glm::rotate(texturemodel, glm::radians(get_time/30), glm::vec3(0.0f, 1.0f, 0.0f));
//        //texturemodel = glm::scale(texturemodel, glm::vec3(ourModel->MIN * 0.6f));
//        //glm::vec3 pos = ourTexturePos->texturePos[0];
//        //texturemodel = glm::translate(texturemodel, -pos);


//        texturemodel = glm::scale(texturemodel,glm::vec3(0.1f));
//        texturemodel = glm::translate(texturemodel, ourModel->MovePos);


//        textureShader->use();
//        textureShader->setMat4("projection", projection);
//        textureShader->setMat4("view", view);

//        textureShader->setMat4("texturemodel", texturemodel);
//        ourTexture->drawTextureDXF();



        if(textureswitch){
            //显示所有贴图
            if(ourDxf == nullptr){
                qDebug() << "未加载贴图";
                textureswitch = false;
            }
            if(textureShader == nullptr){
                qDebug() << "未加载贴图点位";
                textureswitch = false;
            }

            if(ourDxf != nullptr && textureShader != nullptr){
                textureShader->use();
                textureShader->setMat4("projection", projection);
                textureShader->setMat4("view", view);

                unsigned int index = 0;
                for(vector<Point3d>::iterator it = ourTexturePos->texturePos.begin(); it != ourTexturePos->texturePos.end(); ++it){

                    glm::vec3 va = glm::vec3(0.0, 0.0, 1.0);
                    glm::vec3 vb = glm::normalize(textureNormal[index]);
                    index++;

                    glm::vec3 normal = glm::vec3(0.0, 0.0, 0.0);
                    normal = glm::normalize(glm::cross(va,vb));
                    if(va == -vb||va == vb)//如果两个向量共线或者反向，无法算出叉乘
                        normal = glm::vec3(0.0, 0.0, 1.0);

                    float dot = glm::dot(va,vb);
                    float angle = glm::acos(dot);

                    if(vb.z < 0){
                        angle = angle + float(PI);
                    }
                    //angle = angle * 180.0f / PI;

                    texturemodel = glm::mat4(1.0f);

                    texturemodel = glm::rotate(texturemodel, glm::radians(rotateYValue), glm::vec3(0.0f, 1.0f, 0.0f));
                    //texturemodel = glm::rotate(texturemodel, glm::radians(get_time/30), glm::vec3(0.0f, 1.0f, 0.0f));
                    texturemodel = glm::scale(texturemodel, glm::vec3(ourModel->MIN * 0.6f));
                    glm::vec3 pos = *it;
                    texturemodel = glm::translate(texturemodel, 0.15f * vb);//贴图沿法向向外移动一定距离
                    texturemodel = glm::translate(texturemodel, pos);//移到贴图点位相应位置
                    texturemodel = glm::translate(texturemodel, ourModel->MovePos);//移动到模型的原点位置
                    texturemodel = glm::rotate(texturemodel, angle, normal);//旋转模型一定角度
                    textureShader->setMat4("texturemodel", texturemodel);

                    ourTexture->drawTextureDXF();

                    //qDebug() << vb.x << vb.y << vb.z;
                }

            }
        }
            //qDebug() << ourTexturePos->texturePos.size();
    }

    update();
}

void MyWidget::loadModel(string filename)
{
    //qDebug() << "加载模型";
    //const char* path = "C:/Users/chwan/Documents/Qt/OriginalOpenGLProgram/model/平面圆.dxf";
    filename = string(QTextCodec::codecForLocale()->fromUnicode(filename.c_str()).data());
    ourModel = new Model(filename);

    const char *vertexPath = "../OriginalOpenGLProgram/shaders/model.vert";
    const char *fragmentPath = "../OriginalOpenGLProgram/shaders/model.frag";
    ourShader = new Shader(vertexPath, fragmentPath);

    const char *vsPath = "../OriginalOpenGLProgram/shaders/normal.vsh";
    const char *fsPath = "../OriginalOpenGLProgram/shaders/normal.fsh";
    const char *gsPath = "../OriginalOpenGLProgram/shaders/normal.gsh";
    normalShader = new Shader(vsPath, fsPath, gsPath);

    const char *axisvsPath = "../OriginalOpenGLProgram/shaders/axis.vsh";
    const char *axisfsPath = "../OriginalOpenGLProgram/shaders/axis.fsh";
    //const char *axisgsPath = "../OriginalOpenGLProgram/shaders/axis.gsh";
    axisShader = new Shader(axisvsPath, axisfsPath);

//    string dxfPath = "C:/Document/source/测试文件/炮弹/贴图/1-10.dxf";
//    dxfPath = string(QTextCodec::codecForLocale()->fromUnicode(dxfPath.c_str()).data());
//    ourDxf = new DxfReader(dxfPath);

//    string texturePath = "C:/Document/source/测试文件/炮弹/炮弹贴图点位.txt";
//    texturePath = string(QTextCodec::codecForLocale()->fromUnicode(texturePath.c_str()).data());
//    ourTexturePos = new TextureProfile(texturePath);

//    ourTexturePos->calcuTextureNormal(ourModel->only_vertice, ourTexturePos->texturePos, textureNormal);


//    const char *texturevsPath = "C:/Users/chwan/Documents/Qt/OriginalOpenGLProgram/texture.vsh";
//    const char *texturefsPath = "C:/Users/chwan/Documents/Qt/OriginalOpenGLProgram/texture.fsh";
//    textureShader = new Shader(texturevsPath, texturefsPath);

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
    ourModel->MovePos = - ourModel->MiddlePos;

    ourAxis = new AxisMesh();
    //ourTexture = new TextureMesh(ourDxf->pointvec, ourDxf->indices);

    time.start();
}

void MyWidget::shaderMode(int mode)
{
    //渲染模式选择 点模式，线模式，填充模式
    switch (mode) {
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

}

//贴图加载
void MyWidget::loadTexture(string dxfPath)
{
    //string dxfPath = "C:/Document/source/测试文件/炮弹/贴图/1-10.dxf";
    dxfPath = string(QTextCodec::codecForLocale()->fromUnicode(dxfPath.c_str()).data());
    ourDxf = new DxfReader(dxfPath);
}

//贴图点位文件加载
void MyWidget::loadTexturePos(string texturePath)
{
    //string texturePath = "C:/Document/source/测试文件/炮弹/炮弹贴图点位.txt";
    texturePath = string(QTextCodec::codecForLocale()->fromUnicode(texturePath.c_str()).data());
    ourTexturePos = new TextureProfile(texturePath);

    ourTexturePos->calcuTextureNormal(ourModel->only_vertice, ourTexturePos->texturePos, textureNormal);


    const char *texturevsPath = "../OriginalOpenGLProgram/shaders/texture.vsh";
    const char *texturefsPath = "../OriginalOpenGLProgram/shaders/texture.fsh";
    textureShader = new Shader(texturevsPath, texturefsPath);
    ourTexture = new TextureMesh(ourDxf->pointvec, ourDxf->indices);
}

//窗口获取模型文件路径
void MyWidget::Open()
{
    filename = QFileDialog::getOpenFileName(this, "Open the file", "../OriginalOpenGLProgram/model").toStdString();
    if(filename.empty())
        qDebug() << "未加载,请重新加载模型";
    else
        qDebug() << "载入模型: " << filename.substr(filename.find_last_of('/') + 1).c_str();
}

//窗口获取贴图文件路径
void MyWidget::OpenTexture()
{
    dxfPath = QFileDialog::getOpenFileName(this, "Open the texture file", "../OriginalOpenGLProgram/model").toStdString();
    if(dxfPath.empty())
        qDebug() << "未加载,请重新加载贴图文件";
    else
        qDebug() << "载入贴图文件: " << dxfPath.substr(dxfPath.find_last_of('/') + 1).c_str();
}

//窗口获取贴图点位文件路径
void MyWidget::OpenTexturePos()
{
    texturePath = QFileDialog::getOpenFileName(this, "Open the texture position file", "../OriginalOpenGLProgram/model").toStdString();
    if(texturePath.empty())
        qDebug() << "未加载,请重新加载贴图点位文件";
    else
        qDebug() << "载入贴图点位: " << texturePath.substr(texturePath.find_last_of('/') + 1).c_str();
}

//光照开关
void MyWidget::lighting_switch(int state)
{
    if(state == Qt::Checked)
    {
        lightswitch = true;
    }
    else
    {
        lightswitch = false;
    }
    setFocus();
}

//法向量开关
void MyWidget::normal_switch(int state)
{
    if(state == Qt::Checked)
    {
        normalswitch = true;
    }
    else
    {
        normalswitch = false;
    }
    setFocus();
}

//坐标轴开关
void MyWidget::axis_switch(int state)
{
    if(state == Qt::Checked)
    {
        axisswitch = true;
    }
    else
    {
        axisswitch = false;
    }
    setFocus();
}

//贴图显示开关
void MyWidget::texture_switch(int state)
{
    if(state == Qt::Checked)
    {
        textureswitch = true;
    }
    else
    {
        textureswitch = false;
    }
    setFocus();
}

void MyWidget::point_mode(bool checked)
{
    if(checked)
        Select_Mode = 1;
    setFocus();
}

void MyWidget::line_mode(bool checked)
{
    if(checked)
        Select_Mode = 2;
    setFocus();
}

void MyWidget::fill_mode(bool checked)
{
    if(checked)
        Select_Mode = 3;
    setFocus();
}

void MyWidget::resizeGL(int w, int h)
{
    //qDebug() << "resizeGL";
    glViewport(0, 0, w, h);
}

//键盘控制
void MyWidget::keyPressEvent(QKeyEvent *key)
{
    if (key->key()==Qt::Key_Escape) QApplication::exit();
    float cameraSpeed = 2.5f * deltaTime;
    if (key->key()==Qt::Key_W) { cameraPos += cameraSpeed * cameraFront;}
    if (key->key()==Qt::Key_S){ cameraPos -= cameraSpeed * cameraFront;}
    if (key->key()==Qt::Key_A){ cameraPos.x = cameraPos.x + 0.05f;}
    if (key->key()==Qt::Key_D){ cameraPos.x = cameraPos.x - 0.05f;}
    if (key->key()==Qt::Key_L){ lightswitch = !lightswitch;}
    if (key->key()==Qt::Key_N){ normalswitch = !normalswitch;}
    if (key->key()==Qt::Key_N){ axisswitch = !axisswitch;}
    if (key->key()==Qt::Key_1){ Select_Mode = 1;}
    if (key->key()==Qt::Key_2){ Select_Mode = 2;}
    if (key->key()==Qt::Key_3){ Select_Mode = 3;}
    update();

}

//滚轮控制
void MyWidget::wheelEvent(QWheelEvent *event)
{
    float cameraSpeed = 2.5f * deltaTime;
    if(event->delta() > 0)
    {
        cameraPos -= cameraSpeed * cameraFront;;//放大
    }
    else
    {
        cameraPos += cameraSpeed * cameraFront;;//缩小
    }
    update();
}

//鼠标移动控制
void MyWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {//与运算
        if(QApplication::keyboardModifiers() == Qt::ControlModifier)
        {
            QVector2D temp;
            temp = QVector2D(event->localPos()) - startPos;
            //startPos = QVector2D(event->localPos());
            temp = temp.normalized() * temp.length() / 7000.0;

            cameraPos.x -= temp.x();//移动到具体坐标点
            cameraPos.y += temp.y();
    //        ourModel->MovePos.x += temp.x();
    //        ourModel->MovePos.y -= temp.y();
        }
        else {
            float flew = 0.0f;
            flew = event->x() - startX;
            rotateYValue += glm::radians(flew);
            //startX = event->x();
        }

        //qDebug()<<"globalPos"<<event->globalPos()<<endl;
        //qDebug()<<"pos"<<pos()<<endl;
    }

}

//鼠标按压响应
void MyWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        startPos = QVector2D(event->localPos());
        startX = event->x();
    }
}
