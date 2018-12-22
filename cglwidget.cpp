#include "cglwidget.h"
#include "ui_cglwidget.h"

CGLWidget::CGLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::CGLWidget)
{
    ui->setupUi(this);
    rightClicked=false;
    leftClicked=false;
    eyeUp=QVector3D(0.0,1.0,0.0);
    pitch=M_PI_4;
    yaw=0;
    distance=5.0;
    m_update_camera();

    model=new CModel("/home/lee/Pictures/Gundam/StrikeFreedom.obj");
//    model->setRotateX(90);
//    model->setRotateY(90);
skyboxShaderPro=new QOpenGLShaderProgram();
    QTimer *timer=new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(100);
}

CGLWidget::~CGLWidget()
{
    delete ui;
}

void CGLWidget::m_update_camera()
{
    QVector3D Ori=cameraFront+eyePos;
    cameraFront.setX(-cos(pitch)*cos(yaw)*distance);
    cameraFront.setY(-cos(pitch)*sin(yaw)*distance);
    cameraFront.setZ(-sin(pitch)*distance);
    eyePos=Ori-cameraFront;
}
float skyboxVertices[] = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
void CGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    model->initModel();

    skybox=new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);

    QImage img_px=QImage(":/Skybox/right.jpg").convertToFormat(QImage::Format_RGB888);
    QImage img_nx=QImage(":/Skybox/left.jpg").convertToFormat(QImage::Format_RGB888);
    QImage img_py=QImage(":/Skybox/top.jpg").convertToFormat(QImage::Format_RGB888);
    QImage img_ny=QImage(":/Skybox/bottom.jpg").convertToFormat(QImage::Format_RGB888);
    QImage img_pz=QImage(":/Skybox/front.jpg").convertToFormat(QImage::Format_RGB888);
    QImage img_nz=QImage(":/Skybox/back.jpg").convertToFormat(QImage::Format_RGB888);
    skybox->setSize(img_px.width(),img_px.height(),img_px.height());
    skybox->setFormat(QOpenGLTexture::RGBFormat);
    skybox->allocateStorage(QOpenGLTexture::RGB,QOpenGLTexture::UInt8);
    skybox->setData(0,0,QOpenGLTexture::CubeMapPositiveX,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_px.bits());
    skybox->setData(0,0,QOpenGLTexture::CubeMapNegativeX,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_nx.bits());
    skybox->setData(0,0,QOpenGLTexture::CubeMapPositiveY,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_py.bits());
    skybox->setData(0,0,QOpenGLTexture::CubeMapNegativeY,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_ny.bits());
    skybox->setData(0,0,QOpenGLTexture::CubeMapPositiveZ,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_pz.bits());
    skybox->setData(0,0,QOpenGLTexture::CubeMapNegativeZ,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_nz.bits());
    skybox->setMinificationFilter(QOpenGLTexture::Linear);
    skybox->setMagnificationFilter(QOpenGLTexture::Linear);
    skybox->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::ClampToEdge);
    skybox->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::ClampToEdge);
    skybox->setWrapMode(QOpenGLTexture::DirectionR,QOpenGLTexture::ClampToEdge);

    QOpenGLShader *vShader=new QOpenGLShader(QOpenGLShader::Vertex);
    qDebug()<<"Compile skybox Vertex Shader"<<vShader->compileSourceFile(":/skybox.vert");
    QOpenGLShader *fShader=new QOpenGLShader(QOpenGLShader::Fragment);
    qDebug()<<"Compile skybox Fragment Shader"<<fShader->compileSourceFile(":/skybox.frag");
    skyboxShaderPro->create();
    skyboxShaderPro->addShader(vShader);
    skyboxShaderPro->addShader(fShader);
    skyboxShaderPro->link();

    skyboxVBO=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    skyboxVBO->create();
    skyboxVBO->bind();
    skyboxVBO->allocate(skyboxVertices, sizeof(skyboxVertices));
    skyboxVBO->release();
}

void CGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.333,0.333,0.333,1);
    QMatrix4x4 camera;
    eyePos=QVector3D(0,1,-5);
    cameraFront=-eyePos;

    camera.lookAt(eyePos,eyePos+cameraFront,eyeUp);
    model->draw(camera,eyePos);
    glDepthFunc(GL_LEQUAL);
    skyboxShaderPro->bind();
    skyboxVBO->bind();
    skybox->bind(0);
    QMatrix4x4 projection;
    projection.perspective(45.0,aspect,0.005,5000.0);
    skyboxShaderPro->setUniformValue("projection",projection);
    QMatrix4x4 view=camera;
    view.setColumn(3,QVector4D(0,0,0,1));
    skyboxShaderPro->setUniformValue("view",view);
    skyboxShaderPro->setUniformValue("skybox",0);
    skyboxShaderPro->enableAttributeArray("aPos");
    skyboxShaderPro->setAttributeBuffer("aPos",GL_FLOAT,0,3);
    glDrawArrays(GL_TRIANGLES,0,36);
    skyboxShaderPro->disableAttributeArray("aPos");
    skybox->release();
    skyboxVBO->release();
    skyboxShaderPro->release();
    glDepthFunc(GL_LESS);
}

void CGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    float t_aspect=(float)w/(float)h;
    model->setAspect(t_aspect);
    aspect=t_aspect;
}

void CGLWidget::keyPressEvent(QKeyEvent *event)
{

}

void CGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float deltaX=(float)event->x()/(float)width()-lastX;
    float deltaY=(float)event->y()/(float)height()-lastY;
    lastX=(float)event->x()/(float)width();
    lastY=(float)event->y()/(float)height();
    m_mouse_is_move=true;
    if(rightClicked){
        if(eyeUp.z()>0)
            yaw-=deltaX;
        else
            yaw+=deltaX;
        pitch+=deltaY;
        if(pitch>=2*M_PI)
            pitch=pitch-2*M_PI;
        if(pitch<=0)
            pitch=pitch+2*M_PI;
        if(pitch>=M_PI_2&&pitch<3*M_PI_2)
            eyeUp.setZ(-1.0);
        else
            eyeUp.setZ(1.0);
    }
    else if(leftClicked){
        QVector3D norm=QVector3D::crossProduct(cameraFront,eyeUp).normalized();
        eyePos -=  norm * deltaX * distance;
        QVector3D norm2=QVector3D(cameraFront.x(),cameraFront.y(),0.0).normalized();
        if(pitch>=0&&pitch<M_PI_2)
            eyePos +=  norm2 * deltaY * distance;
        else if(pitch>=M_PI_2&&pitch<M_PI)
            eyePos -=  norm2 * deltaY * distance;
        else if(pitch>=M_PI&&pitch<3*M_PI_2)
            eyePos +=  norm2 * deltaY * distance;
        else
            eyePos -=  norm2 * deltaY * distance;
    }
    m_update_camera();
}

void CGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_mouse_is_move=false;
    if(event->button()==Qt::RightButton)
        rightClicked=true;
    if(event->button()==Qt::LeftButton)
        leftClicked=true;
    lastX=(float)event->x()/(float)width();
    lastY=(float)event->y()/(float)height();
}

void CGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
        rightClicked=false;
    if(event->button()==Qt::LeftButton)
        leftClicked=false;
}

void CGLWidget::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y()>0)
        distance+=distance/10.0;
    else if(event->angleDelta().y()<0)
        distance-=distance/10.0;
    if(distance>4000.0)
        distance=4000.0;
    else if(distance<=0.01)
        distance=0.01;
    m_update_camera();
}
