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
driver=new MPU6050Drv();
init_gpio();
#endif


    camera=new CCamera(QVector3D(0,0,-3));
    firstMouse=true;
    lastX=0.0;
    lastY=0.0;
m_main_dir=0;
    skybox=new CSkyBox();
    model=new CModel(QString(path_model)+"StrikeFreedom.obj");
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
        CModel *t_model=new CModel(QString(path_model)+"StrikeFreedom.obj");
        t_model->setPosition(i,-0.5,7.5);
        model_list.push_back(t_model);
    }
}


void CGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    model->initModel();
//    model->setPosition(0,-1.5,2.5);
    skybox->initSkybox();
    for(int i=0;i<model_list.size();i++){
        model_list.at(i)->initModel();
    }

}

void CGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.333,0.333,0.333,1);

#ifdef RASPBERRY_PI
    refesh_mpu6050();
#endif

    QMatrix4x4 t_cam=camera->GetViewMatrix();
    QVector3D t_view_pos=camera->Position;



//    camera.lookAt(eyePos,eyePos+cameraFront,eyeUp);
//     qDebug()<<eyePos<<eyePos+cameraFront;
    for(int i=0;i<model_list.size();i++){
        model_list.at(i)->draw(t_cam,t_view_pos);
    }
    skybox->Draw(t_cam);

//    t_cam=t_cam.inverted();
    QVector3D center=camera->Position + camera->Front;
//    t_cam.setColumn(3,QVector4D(center/*+QVector3D(0,0,-5)*/,1.0));
    QMatrix4x4 mat=t_cam.inverted();
    mat.rotate(180,0,1,0);
    if(m_main_dir==1)
        mat.rotate(20,1,0,0);
    if(m_main_dir==2)
        mat.rotate(-20,1,0,0);
    if(m_main_dir==3)
        mat.rotate(-20,0,0,1);
    if(m_main_dir==4)
        mat.rotate(20,0,0,1);
    mat.setColumn(3,QVector4D(center+QVector3D(0,-0.4,0),1.0));

    model->setMatrix(mat);
//    qDebug()<<"pos:"<<center;

    model->draw(t_cam,t_view_pos);
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
    if (event->key() == Qt::Key_W){
//        m_main_dir=1;
//        camera->ProcessKeyboard(CCamera::FORWARD, 0.1);
        run_forward();
    }
    if (event->key() == Qt::Key_S){
//        m_main_dir=2;
//        camera->ProcessKeyboard(CCamera::BACKWARD, 0.1);
        run_backward();
    }
    if (event->key() == Qt::Key_A){
//        m_main_dir=3;
//        camera->ProcessKeyboard(CCamera::LEFT, 0.1);
        run_left();
    }
    if (event->key() == Qt::Key_D){
//        m_main_dir=4;
//        camera->ProcessKeyboard(CCamera::RIGHT, 0.1);
        run_right();
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
    camera->ProcessKeyboard(CCamera::FORWARD, 0.1);
}

void CGLWidget::run_backward()
{
    m_main_dir=2;
    camera->ProcessKeyboard(CCamera::BACKWARD, 0.1);
}

void CGLWidget::run_left()
{
    m_main_dir=3;
    camera->ProcessKeyboard(CCamera::LEFT, 0.1);
}

void CGLWidget::run_right()
{
    m_main_dir=4;
    camera->ProcessKeyboard(CCamera::RIGHT, 0.1);
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

    if(rot_z<-3.0){
        run_pitch_up();
    }
    else if(rot_z>3.0){
        run_pitch_down();
    }


    if(digitalRead(LEFT_BUTTON_PIN)==HIGH){
        run_yaw_right();
    }
    else if(digitalRead(LEFT_BUTTON_PIN)==HIGH){
        run_yaw_left();
    }


}

void CGLWidget::init_gpio()
{
    pinMode(LEFT_BUTTON_PIN,INPUT);
    pinMode(RIGHT_BUTTON_PIN,INPUT);
    pinMode(FORWARD_BUTTON_PIN,INPUT);
    pinMode(LED_PIN,OUTPUT);

}
