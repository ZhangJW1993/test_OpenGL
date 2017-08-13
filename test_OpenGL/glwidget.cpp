#include "glwidget.h"

#include <QtOpenGL>
#include <QCoreApplication>
#include <GL/glu.h>
//#include <qgl.h>

GLWidget::GLWidget( QWidget* parent, const char* name, bool fs )
    : QGLWidget( parent )
{
    rTri = 0.0;
    rQuad = 0.0;
    fullscreen = fs;
    setGeometry( 0, 0, 640, 480 );
    setWindowTitle( name );
    if ( fullscreen )
        showFullScreen();
}

GLWidget::~GLWidget(){

    // Instantiated only once. Let OS do the cleaning

    makeCurrent();
}

void GLWidget::initializeGL(){

    glShadeModel( GL_SMOOTH );

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    //真正精细的透视修正
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

}

void GLWidget::paintGL()
{
    //clear屏幕和深度缓存。
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //重置当前的模型观察矩阵。
    glLoadIdentity();

    glTranslatef( -1.5,  0.0, -6.0 );
    glRotatef( rTri,  0.0,  1.0,  0.0 );
    glBegin( GL_TRIANGLES );
        glColor3f( 0.0, 0.0, 1.0 );
        glVertex3f(  0.0,  1.0,  0.0 );
        glColor3f( 0.0, 1.0, 0.0 );
        glVertex3f(  -1.0,  -1.0,  0.0 );
        glColor3f( 1.0, 0.0, 0.0 );
        glVertex3f(  1.0,  -1.0,  0.0 );

//        glVertex3f(  1.0,  1.0,  0.0 );
//        glVertex3f(  1.0,  2.0,  0.0 );
//        glVertex3f(  2.0,  2.0,  0.0 );
    glEnd();

    glLoadIdentity();
    glTranslatef(  1.5,  0.0,  -6.0 );
    glRotatef( rQuad,  1.0,  0.0,  0.0 );
//    glTranslatef(  0.0,  0.0,  -4.0 );
    glColor3f( 0.5, 0.5, 1.0 );
    glBegin( GL_QUADS );
        glVertex3f( -1.0,  1.0,  0.0 );
        glVertex3f(  1.0,  1.0,  0.0 );
        glVertex3f(  1.0, -1.0,  0.0 );
        glVertex3f( -1.0, -1.0,  0.0 );
    glEnd();

    rTri += 0.2;
    rQuad -= 0.15;
}

void GLWidget::resizeGL( int width, int height )
{
    //防止height为0。
    if ( height == 0 )
    {
        height = 1;
    }
    //重置当前的视口（Viewport）。
    glViewport( 0, 0, (GLint)width, (GLint)height );
    //选择投影矩阵。
    glMatrixMode( GL_PROJECTION );
    //重置投影矩阵。
    glLoadIdentity();
    //建立透视投影矩阵。
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
    //选择模型观察矩阵。
    glMatrixMode( GL_MODELVIEW );
    //重置模型观察矩阵。
    glLoadIdentity();
}

void GLWidget::keyPressEvent( QKeyEvent *e )
{
    switch ( e->key() )
    {
        case Qt::Key_F2:
            fullscreen = !fullscreen;
            if ( fullscreen )
            {
              showFullScreen();
            }
            else
            {
              showNormal();
              setGeometry( 0, 0, 640, 480 );
            }
            updateGL();
            break;
            //如果按下了F2键，那么屏幕是否全屏的状态就切换一次。然后再根据需要，显示所要的全屏窗口或者普通窗口。

        case Qt::Key_Escape:
            close();   //如果按下了Escape键，程序退出。
    }
}


