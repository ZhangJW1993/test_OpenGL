#include "glwidget.h"

#include <QtOpenGL>
#include <QCoreApplication>
#include <GL/glu.h>
//#include <qgl.h>

GLWidget::GLWidget( QWidget* parent, const char* name, bool fs )
    : QGLWidget( parent )
{
    xRot = yRot = zRot = 0.0;
    zoom = -5.0;
    xSpeed = ySpeed = 0.0;
    filter = 0;
    light = false;

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

    glEnable( GL_TEXTURE_2D );
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.5 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );
}

void GLWidget::paintGL()
{
    //clear屏幕和深度缓存。
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //重置当前的模型观察矩阵。
    glLoadIdentity();

    glTranslatef(  0.0,  0.0, zoom );
    glRotatef( xRot,  1.0,  0.0,  0.0 );
    glRotatef( yRot,  0.0,  1.0,  0.0 );
    glBindTexture( GL_TEXTURE_2D, texture[filter] );
    //draw quads
    glBegin( GL_QUADS );
    //前面。
        glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
    //后面。
        glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
        glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
    //顶面。
        glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
        glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    //底面。
        glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0, -1.0, -1.0 );
        glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    //右面。
        glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
        glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
    //左面。
        glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glEnd();

      xRot += xSpeed;
      yRot += ySpeed;

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
    //按下了L键，就可以切换是否打开光源。
      case Qt::Key_L:
        light = !light;
        if ( !light )
        {
          glDisable( GL_LIGHTING );
        }
        else
        {
          glEnable( GL_LIGHTING );
        }
        updateGL();
        break;
    //按下了F键，就可以转换一下所使用的纹理（就是变换了纹理滤波方式的纹理）。
      case Qt::Key_F:
        filter += 1;;
        if ( filter > 2 )
        {
          filter = 0;
        }
        updateGL();
        break;
    //按下了PageUp键，将木箱移向屏幕内部。
      case Qt::Key_Comma:
        zoom -= 0.2;
        updateGL();
        break;
    //按下了PageDown键，将木箱移向屏幕外部。
      case Qt::Key_Period:
        zoom += 0.2;
        updateGL();
        break;
    //按下了Up方向键，减少xSpeed。
      case Qt::Key_Up:
        xSpeed -= 0.01;
        updateGL();
        break;
    //按下了Dowm方向键，增加xSpeed。
      case Qt::Key_Down:
        xSpeed += 0.01;
        updateGL();
        break;
    //按下了Right方向键，增加ySpeed。
      case Qt::Key_Right:
        ySpeed += 0.01;
        updateGL();
        break;
    //按下了Left方向键，减少ySpeed。
      case Qt::Key_Left:
        ySpeed -= 0.01;
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

void GLWidget::loadGLTextures()
{
    QImage tex, buf;
    //qDebug()<<QDir::currentPath();
    if ( !buf.load( "../data/texture.png" ) )
    {
        qWarning( "Could not read image file, using single-color instead." );
        QImage dummy( 128, 128, QImage::Format_ARGB32 );
        dummy.fill( Qt::green );
        buf = dummy;
    }
    tex = QGLWidget::convertToGLFormat( buf );
    //我们这里创建了3个纹理。
    glGenTextures( 3, &texture[0] );

    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );

    glBindTexture( GL_TEXTURE_2D, texture[1] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );

    //Mipmapping！当您向屏幕绘制一个mipmapped纹理的时候，OpenGL将选择它已经创建的外观最佳的纹理(带有更多细节)来绘制，而不仅仅是缩放原先的图像（这将导致细节丢失）
    glBindTexture( GL_TEXTURE_2D, texture[2] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );

    //可以使用任意的位图来创建纹理。OpenGL将自动将它缩放到正常的大小。

    /**这一行生成 mipmapped 纹理。
     * 使用三种颜色（红，绿，蓝）来生成一个2D纹理。
     * tex.width()是位图宽度，tex.height()是位图高度，
     * extureImage[0]->sizeY 是位图高度，
     * GL_RGBA意味着我们依次使用RGBA色彩。
     * GL_UNSIGNED_BYTE意味着纹理数据的单位是字节。
     * tex.bits()指向我们创建纹理所用的位图。
    **/
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB,
                       tex.width(), tex.height(),
                       GL_RGBA, GL_UNSIGNED_BYTE,
                       tex.bits() );

}
