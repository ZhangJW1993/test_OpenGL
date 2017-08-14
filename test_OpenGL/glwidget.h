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

    void loadGLTextures();

    bool fullscreen;
    GLfloat xRot, yRot, zRot;
    GLfloat zoom;
    GLfloat xSpeed, ySpeed;
    GLuint texture[3];
    GLuint filter;
    bool light;
    GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };
};

#endif // GLWIDGET_H
