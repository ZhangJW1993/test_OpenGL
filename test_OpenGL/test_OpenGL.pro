#-------------------------------------------------
#
# Project created by QtCreator 2017-08-13T16:47:41
#
#-------------------------------------------------

QT       += core gui opengl \
            opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_OpenGL
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        glwidget.cpp

HEADERS  += mainwindow.h\
               glwidget.h

# using pkg-config
linux {
    CONFIG += link_pkgconfig
    PKGCONFIG += glu
}
