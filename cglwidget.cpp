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
    eyePos=QVector3D(0,0,-1);
    cameraFront=-eyePos;

    pitch=M_PI_4;
    yaw=0;
    distance=5.0;
//    m_update_camera();
    skybox=new CSkyBox();
    model=new CModel("/home/lee/Pictures/Gundam/StrikeFreedom.obj");
    create_models();
    QTimer *timer=new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(100);
}

CGLWidget::~CGLWidget()
{
    delete ui;
}

void CGLWidget::create_models()
{
    for(int i=0;i<4;i++){
        CModel *t_model=new CModel("/home/lee/Pictures/Gundam/StrikeFreedom.obj");
        t_model->setPosition(i,-0.5,7.5);
        model_list.push_back(t_model);
    }
}

void CGLWidget::m_update_camera()
{
    QVector3D Ori=cameraFront+eyePos;
    cameraFront.setX(-cos(pitch)*cos(yaw)*distance);
    cameraFront.setY(-cos(pitch)*sin(yaw)*distance);
    cameraFront.setZ(-sin(pitch)*distance);
    eyePos=Ori-cameraFront;
}

void CGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    model->initModel();
    model->setPosition(0,-1.5,2.5);
    skybox->initSkybox();
    for(int i=0;i<model_list.size();i++){
        model_list.at(i)->initModel();
    }

}

void CGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.333,0.333,0.333,1);
    QMatrix4x4 camera;



    camera.lookAt(eyePos,eyePos+cameraFront,eyeUp);
    model->draw(camera,eyePos);
    for(int i=0;i<model_list.size();i++){
        model_list.at(i)->draw(camera,eyePos);
    }
    skybox->Draw(camera);
}

void CGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    float t_aspect=(float)w/(float)h;
    model->setAspect(t_aspect);
    aspect=t_aspect;

    skybox->setAspect(aspect);

    for(int i=0;i<model_list.size();i++){
        model_list.at(i)->setAspect(aspect);
    }
}

void CGLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_W){
        eyePos.setZ(eyePos.z()+0.1);
        model->set_z(model->z()+0.1);
    }
    else if(event->key()==Qt::Key_S){
        eyePos.setZ(eyePos.z()-0.1);
        model->set_z(model->z()-0.1);
    }
    else if(event->key()==Qt::Key_Q){
        QVector3D Ori=cameraFront+eyePos;
        eyePos.setY(eyePos.y()+0.1);
        cameraFront=Ori-cameraFront;
        model->setRotateX(1.0);
    }
    else if(event->key()==Qt::Key_E){
        QVector3D Ori=cameraFront+eyePos;
        eyePos.setY(eyePos.y()-0.1);
        cameraFront=Ori-cameraFront;
        model->setRotateX(-1.0);
    }
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
