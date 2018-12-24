#ifndef CSKYBOX_H
#define CSKYBOX_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
class CSkyBox : public QObject
{
    Q_OBJECT
public:
    explicit CSkyBox(QObject *parent = 0);
    void initSkybox();
    void Draw(QMatrix4x4 _cam);
    void setAspect(float _aspect);
private:
    float aspect;
    QOpenGLTexture *skybox;
    QOpenGLBuffer *skyboxVBO;
    QOpenGLShaderProgram *skyboxShaderPro;
signals:

public slots:
};

#endif // CSKYBOX_H
