#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui/QOpenGLShaderProgram>
#include <QGLWidget>

//这个常量num存储的是我们绘制的星星的总数。
const GLuint num = 50;

typedef struct
{
  int r, g, b;
  GLfloat dist;
  GLfloat angle;
}stars;

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
    void timerEvent( QTimerEvent * );

    void loadGLTextures();

    bool fullscreen;

    GLfloat xRot, yRot, zRot;
    GLfloat zoom;
    GLfloat tilt;
    GLfloat spin;
    GLuint loop;
    GLuint texture[1];

    bool twinkle;

    stars star[num];

};

#endif // GLWIDGET_H
