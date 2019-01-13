#ifndef CGLWIDGET_H
#define CGLWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include "cmodel.h"
#include "cskybox.h"
#include "ccamera.h"
#include <qmath.h>

#include "mpu6050drv.h"

#define LEFT_BUTTON_PIN 2
#define RIGHT_BUTTON_PIN 0
#define FORWARD_BUTTON_PIN 3
#define BACKWARD_BUTTON_PIN 1
#define LED_PIN 7
namespace Ui {
class CGLWidget;
}

class CGLWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CGLWidget(QWidget *parent = 0);
    ~CGLWidget();

    CModel *model;
    CSkyBox *skybox;
    QList<CModel*> model_list;
private:
    void create_models();

//    float lastX,lastY;
//    bool m_mouse_is_move;
//    bool rightClicked,leftClicked;
//    float yaw,pitch;
//    float distance;
//    QVector3D eyePos,eyeUp,cameraFront;
//    void m_update_camera();
    float aspect;
//    int m_main_model_dir;
    CCamera *camera;
    bool firstMouse;
    float lastX,lastY;
    int m_main_dir;

    MPU6050Drv *driver;

    quint32 index;
    CModel *md_f91;
    CModel *md_zaku2;
    CModel *md_f91_2;
    CModel *md_zaku2_2;
    void initOtherModels();
    void run_circle(CModel* _md, quint32 _index, float _rad, float _h, float _bAngle, bool dir);
    void setOtherAspect(float _aspect);
    void drawOtherModels(QMatrix4x4 _camera,QVector3D _cam_pos);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
public:
    void run_forward();
    void run_backward();
    void run_left();
    void run_right();
    void run_pitch_up();
    void run_pitch_down();
    void run_yaw_left();
    void run_yaw_right();

    void refesh_mpu6050();
    void init_gpio();
private:
    Ui::CGLWidget *ui;
};

#endif // CGLWIDGET_H
