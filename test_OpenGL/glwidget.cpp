#include "glwidget.h"

#include <QtOpenGL>
#include <QCoreApplication>
#include <GL/glu.h>
//#include <qgl.h>

GLWidget::GLWidget( QWidget* parent, const char* name, bool fs )
    : QGLWidget( parent )
{
    xRot = yRot = zRot = 0.0;
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

    //启用纹理。如果没有启用的话，你的对象看起来永远都是纯白色。
    glEnable( GL_TEXTURE_2D );

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

    glTranslatef(  0.0,  0.0, -5.0 );
    //根据xRot、yRot、zRot的实际值来旋转正方体。
    glRotatef( xRot,  1.0,  0.0,  0.0 );
    glRotatef( yRot,  0.0,  1.0,  0.0 );
    glRotatef( zRot,  0.0,  0.0,  1.0 );
    //选择我们使用的纹理。如果您在您的场景中使用多个纹理，您应该使用来glBindTexture(GL_TEXTURE_2D, texture[所使用纹理对应的数字]) 选择要绑定的纹理。当您想改变纹理时，应该绑定新的纹理。必须在glBegin()之前或glEnd()之后绑定。
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    //为了将纹理正确的映射到四边形上，您必须将纹理的右上角映射到四边形的右上角，纹理的左上角映射到四边形的左上角，纹理的右下角映射到四边形的右下角，纹理的左下角映射到四边形的左下角。如果映射错误的话，图像显示时可能上下颠倒，侧向一边或者什么都不是。
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

      xRot += 0.3;
      yRot += 0.2;
      zRot += 0.4;
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
  //这里使用了QGLWidget类中提供的一个静态函数converToGLFormat()，专门用来转换图片的，具体情况请参见相应文档。
  tex = QGLWidget::convertToGLFormat( buf ); //convert r and b and Coord y
  /**创建一个纹理。
   * 告诉OpenGL我们想生成一个纹理名字(如果您想载入多个纹理，加大数字)。
  **/
  glGenTextures( 1, &texture[0] );
  /**使用来自位图数据生成的典型纹理。
   * 告诉OpenGL将纹理名字texture[0]绑定到纹理目标上。
   * 2D纹理只有高度（在Y轴上）和宽度（在X轴上）。主函数将纹理名字指派给纹理数据。本例中我们告知OpenGL，&texture[0]处的内存已经可用。我们创建的纹理将存储在&texture[0]的指向的内存区域。
  **/
  glBindTexture( GL_TEXTURE_2D, texture[0] );
  /**这里真正的创建纹理。
   * GL_TEXTURE_2D告诉OpenGL此纹理是一个2D纹理。
   * 数字零代表图像的详细程度，通常就由它为零去了。
   * 数字三是数据的成分数。 因为图像是由红色，绿色，蓝色组成。
   * tex.width()是纹理的宽度。tex.height()是纹理的高度。
   * 数字零是边框的值，一般就是零。
   * GL_RGBA 告诉OpenGL图像数据由红、绿、蓝三色数据以及alpha通道数据组成，这个是QGLWidget类的converToGLFormat()函数的原因。
   * GL_UNSIGNED_BYTE 意味着组成图像的数据是无符号字节类型的。最后tex.bits()告诉OpenGL纹理数据的来源。
  **/
  glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
                GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
  /**告诉OpenGL在显示图像时，当它比放大得原始的纹理大（GL_TEXTURE_MAG_FILTER）或缩小得比原始得纹理小（GL_TEXTURE_MIN_FILTER）时OpenGL采用的滤波方式。
   * 通常这两种情况下我都采用GL_LINEAR。这使得纹理从很远处到离屏幕很近时都平滑显示。使用GL_LINEAR需要CPU和显卡做更多的运算。如果您的机器很慢，您也许应该采用GL_NEAREST。
   * 过滤的纹理在放大的时候，看起来斑驳的很。您也可以结合这两种滤波方式。在近处时使用GL_LINEAR，远处时GL_NEAREST。
  **/
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

}
