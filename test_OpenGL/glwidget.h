#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui/QOpenGLShaderProgram>
#include <QGLWidget>

class GLWidget : public QGLWidget
{
    Q_OBJECT


public:
    GLWidget( QWidget* parent =0, const char* name = 0, bool fs = false );
    ~GLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );
    void keyPressEvent(QKeyEvent *e);

    bool fullscreen;
    GLfloat rTri;
    GLfloat rQuad;
};

#endif // GLWIDGET_H
