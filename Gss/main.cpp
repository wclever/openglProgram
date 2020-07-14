#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QSplashScreen>
#include <QPixmap>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":./images/Qt.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(QStringLiteral("正在载入，请稍等……"), Qt::AlignHCenter|Qt::AlignBottom, Qt::blue);

    QDateTime n = QDateTime::currentDateTime();
    QDateTime now;
    do{
        now = QDateTime::currentDateTime();
        a.processEvents();
    } while (n.secsTo(now) <= 2);//2为需要延时的秒数


    QSurfaceFormat format;
//    format.setRenderableType(QSurfaceFormat::OpenGL);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    format.setVersion(3,3);

    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.resize(QSize(800, 600));
    w.setMinimumSize(QSize(400,300));
    w.setWindowTitle("模型加工系统");
    w.show();

    splash.finish(&w);
    return a.exec();
}
