#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
//    format.setRenderableType(QSurfaceFormat::OpenGL);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    format.setVersion(3,3);

    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.resize(QSize(800, 600));
    w.setMinimumSize(QSize(400,300));
    w.setWindowTitle("opengl window");
    w.show();

    return a.exec();
}
