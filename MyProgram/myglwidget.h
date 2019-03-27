#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
public:
    MyGLWidget(QWidget *parent);
protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);
private:
    const char*  vertexShaderCode =
            "#version 330\r\n"
            ""
            "in layout(location=0) vec3 position;"
            ""
            "void main()"
            "{"
            "   gl_Position = vec4(position, 1.0);"
            "}";

    const char* fragmentShaderCode =
            "#version 330\r\n"
            ""
            "out vec4 daColor;"
            ""
            "void main()"
            "{"
            "   daColor = vec4(1.0, 0.0, 0.0, 1.0);"
            "}";

};

#endif // MYGLWIDGET_H
