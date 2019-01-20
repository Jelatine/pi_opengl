#include "cglwidget.h"
#include "ui_cglwidget.h"
//#define RASPBERRY_PI
#ifdef RASPBERRY_PI
const char* path_model="/home/pi/qt_projects/Gundam/";
#else
const char* path_model="/home/lee/Pictures/Gundam/";
#endif
CGLWidget::CGLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::CGLWidget)
{
    ui->setupUi(this);
    setMouseTracking(true);
//setCursor(Qt::BlankCursor);

#ifdef RASPBERRY_PI
//    showFullScreen();
    driver=new MPU6050Drv();
    init_gpio();
#endif
initCollisionWorld();

index=0;
    camera=new CCamera(QVector3D(0,0,-3));
    firstMouse=true;
    lastX=0.0;
    lastY=0.0;
m_main_dir=0;
    skybox=new CSkyBox();
    model=new CModel(QString(path_model)+"StrikeFreedom.obj",m_dynamicsWorld,1000);

    md_f91=new CModel(QString(path_model)+"f91.obj",m_dynamicsWorld);
//    md_zaku2=new CModel(QString(path_model)+"Zaku2.obj",m_dynamicsWorld);
//    md_f91_2=new CModel(QString(path_model)+"f91.obj",m_dynamicsWorld);
//    md_zaku2_2=new CModel(QString(path_model)+"Zaku2.obj",m_dynamicsWorld);
    create_models();
    QTimer *timer=new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(50);
}

CGLWidget::~CGLWidget()
{
    delete ui;
}

void CGLWidget::create_models()
{
//    for(int i=0;i<4;i++){
//        CModel *t_model=new CModel(QString(path_model)+"StrikeFreedom.obj",m_dynamicsWorld);
//        t_model->setPosition(i,-0.5,7.5);
//        model_list.push_back(t_model);
//    }
}

void CGLWidget::initOtherModels()
{
//    md_f91->initModel();
//    md_zaku2->initModel();
//    md_f91_2->initModel();
//    md_zaku2_2->initModel();
}

void CGLWidget::run_circle(CModel *_md, quint32 _index, float _rad,float _h,float _bAngle,bool dir)
{
    if(dir){
        float x=_rad*sin(qDegreesToRadians((double)_index*1.0+_bAngle));
        float z=_rad*cos(qDegreesToRadians((double)_index*1.0+_bAngle));

        QMatrix4x4 mat;
        mat.setColumn(3,QVector4D(x,_h,z,1.0));
        mat.rotate((double)_index*1.0+90+_bAngle,QVector3D(0,1,0));
        mat.rotate(20,QVector3D(1,0,0));
        _md->setMatrix(mat);
    }
    else{
        float x=_rad*sin(qDegreesToRadians(-(double)_index*1.0));
        float z=_rad*cos(qDegreesToRadians(-(double)_index*1.0+_bAngle));

        QMatrix4x4 mat;
        mat.setColumn(3,QVector4D(x,_h,z,1.0));
        mat.rotate(-(double)_index*1.0-90+_bAngle,QVector3D(0,1,0));
        mat.rotate(20,QVector3D(1,0,0));
        _md->setMatrix(mat);
    }
}

void CGLWidget::setOtherAspect(float _aspect)
{
    md_f91->setAspect(_aspect);
    md_zaku2->setAspect(_aspect);
    md_f91_2->setAspect(_aspect);
    md_zaku2_2->setAspect(_aspect);
}

void CGLWidget::drawOtherModels(QMatrix4x4 _camera, QVector3D _cam_pos)
{
    run_circle(md_f91, index, 10.0,1.0,0.0,true);
    run_circle(md_zaku2, index, 20.0,-1.0,180,false);
    md_f91->draw(_camera,_cam_pos);
    md_zaku2->draw(_camera,_cam_pos);

    run_circle(md_f91_2, index, 30.0,0.0,0.-90,false);
    run_circle(md_zaku2_2, index, 40.0,2.0,90,true);
    md_f91_2->draw(_camera,_cam_pos);
    md_zaku2_2->draw(_camera,_cam_pos);
    index++;
}

void CGLWidget::initCollisionWorld()
{
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, 0, 0));
}


void CGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    model->initModel();
    md_f91->initModel();
    md_f91->setPosition(0,-0.0,4.5);
//    model->setPosition(0,-1.5,2.5);
    skybox->initSkybox();
//    for(int i=0;i<model_list.size();i++){
//        model_list.at(i)->initModel();
//    }
    initOtherModels();
}

void CGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.333,0.333,0.333,1);

#ifdef RASPBERRY_PI
    refesh_mpu6050();
#endif
m_dynamicsWorld->stepSimulation(1.f / 60.f);
    QMatrix4x4 t_cam=camera->GetViewMatrix();
    QVector3D t_view_pos=camera->Position;



//    camera.lookAt(eyePos,eyePos+cameraFront,eyeUp);
//     qDebug()<<eyePos<<eyePos+cameraFront;
//    for(int i=0;i<model_list.size();i++){
//        model_list.at(i)->draw(t_cam,t_view_pos);
//    }
    skybox->Draw(t_cam);

//    t_cam=t_cam.inverted();
    QVector3D center=camera->Position + camera->Front;
//    t_cam.setColumn(3,QVector4D(center/*+QVector3D(0,0,-5)*/,1.0));
    QMatrix4x4 mat=t_cam.inverted();
    mat.rotate(180,0,1,0);
    if(m_main_dir==1){
        mat.rotate(20,1,0,0);
    }
    if(m_main_dir==2)
        mat.rotate(-20,1,0,0);
    if(m_main_dir==3)
        mat.rotate(-20,0,0,1);
    if(m_main_dir==4)
        mat.rotate(20,0,0,1);
    camera->setTargetPosition(model->getPosition());
    mat.setColumn(3,QVector4D(center+QVector3D(0,-0.4,0),1.0));

//    model->setMatrix(mat);
//    qDebug()<<"pos:"<<center;

    model->draw(t_cam,t_view_pos);
    md_f91->draw(t_cam,t_view_pos);
//    drawOtherModels(t_cam,t_view_pos);
}

void CGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    float t_aspect=(float)w/(float)h;
    model->setAspect(t_aspect);
    md_f91->setAspect(t_aspect);
    aspect=t_aspect;

    skybox->setAspect(aspect);

//    for(int i=0;i<model_list.size();i++){
//        model_list.at(i)->setAspect(aspect);
//    }

//    setOtherAspect(aspect);
}

void CGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W){
//        m_main_dir=1;
//        camera->ProcessKeyboard(CCamera::FORWARD, 0.1);
        run_forward();
//        model->set_velocity(0,0,10);
        model->m_body->setLinearVelocity(btVector3(0,0,10));
    }
    if (event->key() == Qt::Key_S){
//        m_main_dir=2;
//        camera->ProcessKeyboard(CCamera::BACKWARD, 0.1);
        run_backward();
        model->m_body->setLinearVelocity(btVector3(0,0,-10));
    }
    if (event->key() == Qt::Key_A){
//        m_main_dir=3;
//        camera->ProcessKeyboard(CCamera::LEFT, 0.1);
        run_left();

        model->m_body->setLinearVelocity(btVector3(10,0,0));
    }
    if (event->key() == Qt::Key_D){
//        m_main_dir=4;
//        camera->ProcessKeyboard(CCamera::RIGHT, 0.1);
        run_right();
        model->m_body->setLinearVelocity(btVector3(-10,0,0));
    }


    if (event->key() == Qt::Key_R){
        run_pitch_up();
    }

    if (event->key() == Qt::Key_F){
        run_pitch_down();
    }

    if (event->key() == Qt::Key_Q){
        run_yaw_left();
    }

    if (event->key() == Qt::Key_E){
        run_yaw_right();
    }
    if (event->key() == Qt::Key_Escape){
        close();
    }
}

void CGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    m_main_dir=0;

    model->m_body->setLinearVelocity(btVector3(0,0,0));
}

void CGLWidget::mouseMoveEvent(QMouseEvent *event)
{
//    if (firstMouse)
//    {
//        lastX = (float)event->x();
//        lastY = (float)event->y();
//        firstMouse = false;
//    }

//    float xoffset = (float)event->x() - lastX;
//    float yoffset = lastY - (float)event->y(); // reversed since y-coordinates go from bottom to top

//    lastX = (float)event->x();
//    lastY = (float)event->y();

//    camera->ProcessMouseMovement(xoffset, yoffset);
#ifndef RASPBERRY_PI
    float xoffset = (float)event->x() - width()/2.0;
    float yoffset = height()/2.0 - (float)event->y();
    camera->ProcessMouseMovement(xoffset, yoffset);
    QCursor::setPos(mapToGlobal(QPoint(width()/2,height()/2)));
#endif
}

void CGLWidget::mousePressEvent(QMouseEvent *event)
{
}

void CGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
}

void CGLWidget::wheelEvent(QWheelEvent *event)
{
}

void CGLWidget::run_forward()
{
    m_main_dir=1;
//    camera->ProcessKeyboard(CCamera::FORWARD, 0.1);
}

void CGLWidget::run_backward()
{
    m_main_dir=2;
//    camera->ProcessKeyboard(CCamera::BACKWARD, 0.1);
}

void CGLWidget::run_left()
{
    m_main_dir=3;
//    camera->ProcessKeyboard(CCamera::LEFT, 0.1);
}

void CGLWidget::run_right()
{
    m_main_dir=4;
//    camera->ProcessKeyboard(CCamera::RIGHT, 0.1);
}

void CGLWidget::run_pitch_up()
{
    camera->ProcessMouseMovement(0, 20);
}

void CGLWidget::run_pitch_down()
{
    camera->ProcessMouseMovement(0, -20);
}

void CGLWidget::run_yaw_left()
{
    camera->ProcessMouseMovement(-20, 0);
}

void CGLWidget::run_yaw_right()
{
    camera->ProcessMouseMovement(20, 0);
}

void CGLWidget::refesh_mpu6050()
{
    std::vector<float> result=driver->getAccData();
    float rot_y=result.at(1);
    float rot_z=result.at(2);
    if(rot_y<-3.0){
        run_right();
    }
    else if(rot_y>3.0){
        run_left();
    }
    else{
        m_main_dir=0;
    }

    if(rot_z<-3.0){
        run_pitch_up();
    }
    else if(rot_z>3.0){
        run_pitch_down();
    }


    if(digitalRead(LEFT_BUTTON_PIN)==HIGH){
        run_yaw_right();
    }
    else if(digitalRead(RIGHT_BUTTON_PIN)==HIGH){
        run_yaw_left();
    }
    else if(digitalRead(FORWARD_BUTTON_PIN)==HIGH){
        run_forward();
    }
    else if(digitalRead(BACKWARD_BUTTON_PIN)==HIGH){
        run_backward();
    }

    if(m_main_dir==1){
        digitalWrite(LED_PIN,LOW);
        run_forward();
    }
    else if(m_main_dir==2){
        run_backward();
        digitalWrite(LED_PIN,HIGH);
    }
    else if(m_main_dir==3||m_main_dir==4)
    {
        digitalWrite(LED_PIN,HIGH);
    }
    else{
        digitalWrite(LED_PIN,HIGH);
        m_main_dir=0;
    }


}

void CGLWidget::init_gpio()
{
    pinMode(LEFT_BUTTON_PIN,INPUT);
    pinMode(RIGHT_BUTTON_PIN,INPUT);
    pinMode(FORWARD_BUTTON_PIN,INPUT);
    pinMode(BACKWARD_BUTTON_PIN,INPUT);
    pinMode(LED_PIN,OUTPUT);

}
