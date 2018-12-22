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
private:
    float lastX,lastY;
    bool m_mouse_is_move;
    bool rightClicked,leftClicked;
    float yaw,pitch;
    float distance;
    QVector3D eyePos,eyeUp,cameraFront;
    QOpenGLTexture *skybox;
    QOpenGLBuffer *skyboxVBO;
    QOpenGLShaderProgram *skyboxShaderPro;
    void m_update_camera();
    float aspect;
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    Ui::CGLWidget *ui;
};

#endif // CGLWIDGET_H
