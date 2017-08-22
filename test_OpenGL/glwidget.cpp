#include "glwidget.h"

#include <QtOpenGL>
#include <QCoreApplication>
#include <GL/glu.h>
//#include <qgl.h>

GLWidget::GLWidget( QWidget* parent, const char* name, bool fs )
    : QGLWidget( parent )
{
    xRot = yRot = zRot = 0.0;
    zoom = -15.0;
    tilt = 90.0;
    spin = 0.0;
    loop = 0;

    twinkle = false;

    fullscreen = fs;
    setGeometry( 0, 0, 640, 480 );
    setWindowTitle( name );

    if ( fullscreen )
    showFullScreen();

    startTimer( 5 );
}

GLWidget::~GLWidget(){

    // Instantiated only once. Let OS do the cleaning

    makeCurrent();
}

void GLWidget::initializeGL(){

    //载入纹理。
    loadGLTextures();
    glEnable( GL_TEXTURE_2D );
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.5 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    glEnable( GL_BLEND );
    for ( loop = 0; loop < num; loop++ )
    {
        star[loop].angle = 0.0;
        star[loop].dist = ( float(loop)/num ) * 5.0;
        star[loop].r = rand() % 256;
        star[loop].g = rand() % 256;
        star[loop].b = rand() % 256;
    }
}

void GLWidget::paintGL()
{
    //clear屏幕和深度缓存。
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBindTexture( GL_TEXTURE_2D, texture[0] );
    for ( loop = 0; loop < num; loop++ )
    {
        //绘制每颗星星之前，重置模型观察矩阵
        glLoadIdentity();
        glTranslatef( 0.0, 0.0, zoom );
        glRotatef( tilt, 1.0, 0.0, 0.0 );
        glRotatef( star[loop].angle, 0.0, 1.0, 0.0 );
        glTranslatef( star[loop].dist, 0.0, 0.0 );  //
        glRotatef( -star[loop].angle, 0.0, 1.0, 0.0 );
        glRotatef( -tilt, 1.0, 0.0, 0.0 );

        if ( twinkle )
        {
            glColor4ub( star[(num-loop)-1].r,
                star[(num-loop)-1].g,
                star[(num-loop)-1].b, 255 );
            glBegin( GL_QUADS );
                glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, 0.0 );
                glTexCoord2f( 1.0, 0.0 ); glVertex3f( 1.0, -1.0, 0.0 );
                glTexCoord2f( 1.0, 1.0 ); glVertex3f( 1.0, 1.0, 0.0 );
                glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0, 1.0, 0.0 );
            glEnd();
        }
        glRotatef( spin, 0.0, 0.0, 1.0 );
        glColor4ub( star[loop].r, star[loop].g, star[loop].b, 255 );
        glBegin( GL_QUADS );
            glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, 0.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex3f( 1.0, -1.0, 0.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex3f( 1.0, 1.0, 0.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0, 1.0, 0.0 );
        glEnd();

        spin += 0.01;
        star[loop].angle += float(loop)/num;
        star[loop].dist -= 0.01;

        if ( star[loop].dist < 0.0 )
        {
            star[loop].dist += 5.0;
            star[loop].r = rand() % 256;
            star[loop].g = rand() % 256;
            star[loop].b = rand() % 256;
        }
    }

    update();
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
        case Qt::Key_T:
            twinkle = !twinkle;
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
    if ( !buf.load( "../data/Star.bmp" ) )
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
