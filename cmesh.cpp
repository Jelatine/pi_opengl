#include "cmesh.h"

CMesh::CMesh(){

}

CMesh::CMesh(QVector<Vertex> vertices, QVector<unsigned int> indices, Texture textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    m_texture_diffuse=NULL;
    m_texture_specular=NULL;
    m_texture_emission=NULL;
    VBO=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    EBO=new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
}

void CMesh::setupMesh(){
//    qDebug()<<"textures.diffuse"<<textures.diffuse  ;
//    qDebug()<<"textures.ambient"<<textures.ambient  ;
//    qDebug()<<"textures.specular"<<textures.specular ;
//    qDebug()<<"textures.shininess"<<textures.shininess;

    if(!this->textures.diffuse_path.isEmpty()){
        QImage img_diffuse(this->textures.diffuse_path);
        if(!img_diffuse.isNull()){
            m_texture_diffuse=new QOpenGLTexture(img_diffuse.mirrored());}}
    if(!this->textures.specular_path.isEmpty()){
        QImage img_specular(this->textures.specular_path);
        if(!img_specular.isNull()){
            m_texture_specular=new QOpenGLTexture(img_specular.mirrored());}}
    if(!this->textures.emission_path.isEmpty()){
        QImage img_emission(this->textures.emission_path);
        if(!img_emission.isNull()){
            m_texture_emission=new QOpenGLTexture(img_emission.mirrored());}}
    VBO=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    VBO->create();
    VBO->bind();
    VBO->allocate(vertices.constData(), vertices.size() * sizeof(Vertex));
    VBO->release();
    EBO=new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    EBO->create();
    EBO->bind();
    EBO->allocate(indices.constData(),indices.size() * sizeof(unsigned int));
    EBO->release();
}

void CMesh::Draw(QOpenGLShaderProgram *shader){
    VBO->bind();
    EBO->bind();
    shader->setUniformValue("material.diffuse",   textures.diffuse  );
    shader->setUniformValue("material.ambient",   textures.ambient  );
    shader->setUniformValue("material.specular",  textures.specular );
    shader->setUniformValue("material.shininess", textures.shininess);
    shader->setUniformValue("TexCoords",0);
    shader->enableAttributeArray("aPos");
    shader->setAttributeBuffer("aPos",GL_FLOAT,0,3,sizeof(Vertex));
    shader->enableAttributeArray("aNormal");
    shader->setAttributeBuffer("aNormal",GL_FLOAT,offsetof(Vertex, Normal),3,sizeof(Vertex));
    shader->enableAttributeArray("aTexCoords");
    shader->setAttributeBuffer(2,GL_FLOAT,offsetof(Vertex, TexCoords),2,sizeof(Vertex));
    shader->setUniformValue("material.en_diffuse_tex", 0);
    shader->setUniformValue("material.en_specular_tex", 0);
    shader->setUniformValue("material.en_emission_tex", 0);


    if(m_texture_diffuse!=NULL){
        shader->setUniformValue("material.en_diffuse_tex", 1);
        m_texture_diffuse->bind(1);
    }
    if(m_texture_emission!=NULL){
        shader->setUniformValue("material.en_emission_tex", 1);
        m_texture_specular->bind(2);
    }
    if(m_texture_emission!=NULL){
        shader->setUniformValue("material.en_emission_tex", 1);
        m_texture_emission->bind(3);
    }
    shader->setUniformValue("material.tex_diffuse", 1);
    shader->setUniformValue("material.tex_specular", 2);
    shader->setUniformValue("material.tex_emission", 3);
    QOpenGLContext::currentContext()->functions()->glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT,0);
    VBO->release();
    EBO->release();
    if(m_texture_diffuse!=NULL)m_texture_diffuse->release();
    if(m_texture_diffuse!=NULL)m_texture_diffuse->release();
    if(m_texture_diffuse!=NULL)m_texture_diffuse->release();
}

void CMesh::m_set_ambient(QVector3D _ambient)
{
    textures.ambient=_ambient;
}

void CMesh::m_set_diffuse(QVector3D _diffuse)
{
    textures.diffuse=_diffuse;
}

void CMesh::m_set_specular(QVector3D _specular)
{
    textures.specular=_specular;
}

void CMesh::m_set_shininess(float _shininess)
{
    textures.shininess=_shininess;
}



