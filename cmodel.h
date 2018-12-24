#ifndef CMODEL_H
#define CMODEL_H

#include <QObject>
#include <QDebug>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QFile>
#include "cmesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
using namespace Assimp;

class CModel : public QObject
{
    Q_OBJECT
public:
    explicit CModel(QString _name, bool gamma = false,QObject *parent = 0);
    void loadModel(QString _name);
    void initModel();
    void draw(QMatrix4x4 _camera, QVector3D _cam_pos);
    QOpenGLShaderProgram *shaderProgram;
    QVector<CMesh> meshes;
    QVector<Texture> textures_loaded;
    QString directory;
    bool gammaCorrection;
    void processNode(aiNode *node, const aiScene *scene);
    CMesh processMesh(aiMesh *mesh, const aiScene *scene);
    QString m_format_path(QString filename);
    QMatrix4x4 model;
    void setRotateX(float angle);
    void setRotateY(float angle);
    void setRotateZ(float angle);
    float aspect;
    void setAspect(float _aspect);
    void setPosition(float _x,float _y,float _z);
    float x();
    float y();
    float z();
    void set_x(float _x);
    void set_y(float _y);
    void set_z(float _z);

signals:

public slots:
};

#endif // CMODEL_H
