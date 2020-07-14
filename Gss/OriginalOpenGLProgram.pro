#-------------------------------------------------
#
# Project created by QtCreator 2019-03-24T15:09:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += $$PWD/lib/libassimp.dll.a \
        $$PWD/lib/libdxfrw.dll.a

INCLUDEPATH += ./include

TARGET = OriginalOpenGLProgram
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mywidget.cpp \
    shader.cpp \
    mesh.cpp \
    model.cpp \
    axismesh.cpp \
    dxfreader.cpp \
    dx_iface.cpp \
    textureprofile.cpp \
    texturemesh.cpp

HEADERS += \
        mainwindow.h \
    mywidget.h \
    shader.h \
    mesh.h \
    model.h \
    axismesh.h \
    dxfreader.h \
    dx_data.h \
    dx_iface.h \
    datastruct.h \
    textureprofile.h \
    texturemesh.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc \
    shaders.qrc
