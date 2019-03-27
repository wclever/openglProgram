#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
{
    setSurfaceType(QWindow::OpenGLSurface);

    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2,1);
    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();
    context->makeCurrent(this);

    openGLFunctions = context->functions();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateAnimation()));
    timer->start(100);

    rotation = 0;
}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    resizeGL(this->width(), this->height());
}

void MainWindow::resizeGL(int w, int h)
{
    //set viewport
    glViewport(0, 0, w, h);
    qreal aspectratio = qreal(w) / qreal(h);

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75, aspectratio, 0.1, 400000000);

    //Initialize Model View Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glOrtho(-1 * aspectratio, 1 * aspectratio, -1,1,1,-1);
}

void MainWindow::paintGL()
{
    glClearColor(0.5, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glutWireTeapot(0.6);
    //Reset Model View Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 3D Transformation

    glTranslated(0.0, 0.0, -3.0);
    glRotatef(rotation, 1.0, 1.0, 1.0);

    // FRONT
   glBegin(GL_LINE_LOOP);
       glColor3f(0.0,  0.0, 0.0);
       glVertex3f(0.5, -0.5, -0.5);
       glVertex3f(0.5, 0.5, -0.5);
       glVertex3f(-0.5, 0.5, -0.5);
       glVertex3f(-0.5, -0.5, -0.5);
   glEnd();

   // BACK
   glBegin(GL_LINE_LOOP);
       glColor3f(0.0,  1.0, 0.0);
       glVertex3f(0.5, -0.5, 0.5);
       glVertex3f(0.5, 0.5, 0.5);
       glVertex3f(-0.5, 0.5, 0.5);
       glVertex3f(-0.5, -0.5, 0.5);
   glEnd();

   // RIGHT
   glBegin(GL_LINE_LOOP);
       glColor3f(1.0, 0.0, 1.0);
       glVertex3f(0.5, -0.5, -0.5);
       glVertex3f(0.5, 0.5, -0.5);
       glVertex3f(0.5, 0.5, 0.5);
       glVertex3f(0.5, -0.5, 0.5);
   glEnd();

   // LEFT
   glBegin(GL_LINE_LOOP);
       glColor3f(1.0, 1.0, 0.0);
       glVertex3f(-0.5, -0.5, 0.5);
       glVertex3f(-0.5, 0.5, 0.5);
       glVertex3f(-0.5, 0.5, -0.5);
       glVertex3f(-0.5, -0.5, -0.5);
   glEnd();

   // TOP
   glBegin(GL_LINE_LOOP);
       glColor3f(0.0, 0.0, 1.0);
       glVertex3f(0.5, 0.5, 0.5);
       glVertex3f(0.5, 0.5, -0.5);
       glVertex3f(-0.5, 0.5, -0.5);
       glVertex3f(-0.5, 0.5, 0.5);
   glEnd();

   // Red side - BOTTOM
   glBegin(GL_LINE_LOOP);
       glColor3f(1.0, 0.0, 0.0);
       glVertex3f(0.5, -0.5, -0.5);
       glVertex3f(0.5, -0.5, 0.5);
       glVertex3f(-0.5, -0.5, 0.5);
       glVertex3f(-0.5, -0.5, -0.5);
   glEnd();
    /*glBegin(GL_QUADS);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.5f, -0.5f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(0.5f, 0.5f);

    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);

    glEnd();*/

    glFlush();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeGL(this->width(), this->height());
    this->update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    paintGL();
}

void MainWindow::UpdateAnimation()
{
    rotation += 10;
    this->update();
}
