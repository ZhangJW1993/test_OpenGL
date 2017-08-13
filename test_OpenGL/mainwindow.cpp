#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    glwidget = new GLWidget(this, "GLWidget", false);
    setCentralWidget(glwidget);
}
