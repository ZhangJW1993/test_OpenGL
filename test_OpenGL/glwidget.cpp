#include "glwidget.h"

#include <math.h>
#include <QtOpenGL>
#include <QCoreApplication>
#include <GL/glu.h>
//#include <qgl.h>
static GLfloat boxcol[5][3] =
{
  { 1.0, 0.0, 0.0 },
  { 1.0, 0.5, 0.0 },
  { 1.0, 1.0, 0.0 },
  { 0.0, 1.0, 0.0 },
  { 0.0, 1.0, 1.0 }
};

static GLfloat topcol[5][3] =
{
  { 0.5, 0.0, 0.0 },
  { 0.5, 0.25, 0.0 },
  { 0.5, 0.5, 0.0 },
  { 0.0, 0.5, 0.0 },
  { 0.0, 0.5, 0.5 }
};

GLWidget::GLWidget( QWidget* parent, const char* name, bool fs )
    : QGLWidget( parent )
{
    xRot = yRot = zRot = 0.0;
    box = top = 0;

    xLoop = yLoop = 0;

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

    //载入纹理。
    loadGLTextures();
    buildLists();
    glEnable( GL_TEXTURE_2D );
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.5 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHTING );
    glEnable( GL_COLOR_MATERIAL );

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

}

void GLWidget::paintGL()
{
    //clear屏幕和深度缓存。
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindTexture( GL_TEXTURE_2D, texture[0] );

    for ( yLoop = 1; yLoop < 6; yLoop++ )
    {
        for ( xLoop = 0; xLoop < yLoop; xLoop++ )
        {
          glLoadIdentity();
          glTranslatef( 1.4 + (float(xLoop) * 2.8) - (float(yLoop) * 1.4),
              ( (6.0 - (float(yLoop)) ) * 2.4 ) - 7.0, -20.0 );
          glRotatef( 45.0 - (2.0 * yLoop) + xRot, 1.0, 0.0, 0.0 );
          glRotatef( 45.0 + yRot, 0.0, 1.0, 0.0 );
          glColor3fv( boxcol[yLoop-1] );
          glCallList( box );

          glColor3fv( topcol[yLoop-1] );
          glCallList( top );
        }
    }
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
        case Qt::Key_Up:
            xRot -= 0.2;
            updateGL();
        break;
        case Qt::Key_Down:
            xRot += 0.2;
            updateGL();
        break;
        case Qt::Key_Left:
            yRot -= 0.2;
            updateGL();
        break;
        case Qt::Key_Right:
            yRot += 0.2;
            updateGL();
        break;
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
            update();
        break;
        case Qt::Key_Escape:
            close();
    }
}

void GLWidget::timerEvent(QTimerEvent*)
{

    updateGL();
}

void GLWidget::loadGLTextures()
{
    QImage tex, buf;
    //qDebug()<<QDir::currentPath();
    if ( !buf.load( "../data/Cube.bmp" ) )
    {
        qWarning( "Could not read image file, using single-color instead." );
        QImage dummy( 128, 128, QImage::Format_ARGB32 );
        dummy.fill( Qt::green );
        buf = dummy;
    }
    tex = QGLWidget::convertToGLFormat( buf );
    //我们这里创建了3个纹理。
    glGenTextures( 1, &texture[0] );

    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );                            

}

void GLWidget::buildLists()
{
    box = glGenLists( 2 );
    glNewList( box, GL_COMPILE );
        //画一个没有顶部的盒子，它不会出现在屏幕上，只会存储在显示列表里。
        glBegin( GL_QUADS );
            glNormal3f( 0.0, -1.0, 0.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0, -1.0, -1.0 );
            glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );

            glNormal3f( 0.0, 0.0, 1.0 );
            glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );

            glNormal3f( 0.0, 0.0, -1.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
            glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );

            glNormal3f( 1.0, 0.0, 0.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
            glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

            glNormal3f( -1.0, 0.0, 0.0 );
            glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
          glEnd();
      glEndList();

      //建立第二个显示列表。在上一个显示列表的指针上加1，就得到了第二个显示列表的指针。
      top = box + 1;
      glNewList( top, GL_COMPILE );
        glBegin( GL_QUADS );
        glNormal3f( 0.0, 1.0, 0.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
            glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
        glEnd();
      glEndList();

}
