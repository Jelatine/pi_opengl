#ifndef CMESH_H
#define CMESH_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

struct Vertex {
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;};
struct Texture {
    QString diffuse_path;
    QString specular_path;
    QString emission_path;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    float shininess;};
class CMesh {
public:
    CMesh();
    CMesh(QVector<Vertex> vertices, QVector<unsigned int> indices, Texture textures);public:
    void setupMesh();
    void Draw(QOpenGLShaderProgram *shader);
    void m_set_ambient(QVector3D _ambient);
    void m_set_diffuse(QVector3D _diffuse);
    void m_set_specular(QVector3D _specular);
    void m_set_shininess(float _shininess);
private:
    QOpenGLBuffer *VBO,*EBO;
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    Texture textures;
    QOpenGLTexture *m_texture_diffuse;
    QOpenGLTexture *m_texture_specular;
    QOpenGLTexture *m_texture_emission;
//    unsigned int VAO;
};

#endif // CMESH_H
