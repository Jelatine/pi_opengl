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

//#include <bullet/btBulletCollisionCommon.h>
//#include <btBulletCollisionCommon.h>
//#include <BulletCollision/btBulletCollisionCommon.h>

#include <btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/btBulletCollisionCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btTriangleShape.h>
#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
class CModel
{
public:
    explicit CModel(QString _name, btDiscreteDynamicsWorld* _world=0, float _mass=1.0);
    void loadModel(QString _name);
    void initModel();
    void draw(QMatrix4x4 _camera, QVector3D _cam_pos);
    QOpenGLShaderProgram *shaderProgram;
    QVector<CMesh> meshes;
    QVector<Texture> textures_loaded;
    QString directory;
//    bool gammaCorrection;
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
    QVector3D getPosition();
    float x();
    float y();
    float z();
    void set_x(float _x);
    void set_y(float _y);
    void set_z(float _z);
    QMatrix4x4 getMatrix();
    void setMatrix(QMatrix4x4 _model);

    float m_mass;
    btCompoundShape *m_compound_shape;
    btRigidBody *m_body;
    btDiscreteDynamicsWorld* m_world;
    void set_velocity(float x,float y,float z);
signals:

public slots:
};

#endif // CMODEL_H
