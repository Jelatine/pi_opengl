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
private:
    Ui::CGLWidget *ui;
};

#endif // CGLWIDGET_H
