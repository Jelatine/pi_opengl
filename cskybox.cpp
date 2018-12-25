#include "cskybox.h"

CSkyBox::CSkyBox(QObject *parent) : QObject(parent)
{
    aspect=1.0;
    skyboxShaderPro=new QOpenGLShaderProgram();
}
float skyboxVertices[] = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
void CSkyBox::initSkybox()
{
    skybox=new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);

    QImage img_px=QImage(":/Skybox/right.jpg").convertToFormat(QImage::Format_RGB888);
    QImage img_nx=QImage(":/Skybox/left.jpg").convertToFormat(QImage::Format_RGB888);
    QImage img_py=QImage(":/Skybox/top.jpg").convertToFormat(QImage::Format_RGB888);
    QImage img_ny=QImage(":/Skybox/bottom.jpg").convertToFormat(QImage::Format_RGB888);
    QImage img_pz=QImage(":/Skybox/front.jpg").convertToFormat(QImage::Format_RGB888);
    QImage img_nz=QImage(":/Skybox/back.jpg").convertToFormat(QImage::Format_RGB888);
    skybox->setSize(img_px.width(),img_px.height(),img_px.height());
    skybox->setFormat(QOpenGLTexture::RGBFormat);
    skybox->allocateStorage(QOpenGLTexture::RGB,QOpenGLTexture::UInt8);
    skybox->setData(0,0,QOpenGLTexture::CubeMapPositiveX,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_px.bits());
    skybox->setData(0,0,QOpenGLTexture::CubeMapNegativeX,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_nx.bits());
    skybox->setData(0,0,QOpenGLTexture::CubeMapPositiveY,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_py.bits());
    skybox->setData(0,0,QOpenGLTexture::CubeMapNegativeY,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_ny.bits());
    skybox->setData(0,0,QOpenGLTexture::CubeMapPositiveZ,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_pz.bits());
    skybox->setData(0,0,QOpenGLTexture::CubeMapNegativeZ,QOpenGLTexture::RGB,QOpenGLTexture::UInt8,(const void*)img_nz.bits());
    skybox->setMinificationFilter(QOpenGLTexture::Linear);
    skybox->setMagnificationFilter(QOpenGLTexture::Linear);
    skybox->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::ClampToEdge);
    skybox->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::ClampToEdge);
//    skybox->setWrapMode(QOpenGLTexture::DirectionR,QOpenGLTexture::ClampToEdge);

    QOpenGLShader *vShader=new QOpenGLShader(QOpenGLShader::Vertex);
    qDebug()<<"Compile skybox Vertex Shader"<<vShader->compileSourceFile(":/skybox.vert");
    QOpenGLShader *fShader=new QOpenGLShader(QOpenGLShader::Fragment);
    qDebug()<<"Compile skybox Fragment Shader"<<fShader->compileSourceFile(":/skybox.frag");
    skyboxShaderPro->create();
    skyboxShaderPro->addShader(vShader);
    skyboxShaderPro->addShader(fShader);
    skyboxShaderPro->link();

    skyboxVBO=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    skyboxVBO->create();
    skyboxVBO->bind();
    skyboxVBO->allocate(skyboxVertices, sizeof(skyboxVertices));
    skyboxVBO->release();
}

void CSkyBox::Draw(QMatrix4x4 _cam)
{
    QOpenGLContext::currentContext()->functions()->glDepthFunc(GL_LEQUAL);
    skyboxShaderPro->bind();
    skyboxVBO->bind();
    skybox->bind(0);
    QMatrix4x4 projection;
    projection.perspective(45.0,aspect,0.1,5000.0);
    skyboxShaderPro->setUniformValue("projection",projection);
    QMatrix4x4 view=_cam;
    view.setColumn(3,QVector4D(0,0,0,1));
    skyboxShaderPro->setUniformValue("view",view);
    skyboxShaderPro->setUniformValue("skybox",0);
    skyboxShaderPro->enableAttributeArray("aPos");
    skyboxShaderPro->setAttributeBuffer("aPos",GL_FLOAT,0,3);
    QOpenGLContext::currentContext()->functions()->glDrawArrays(GL_TRIANGLES,0,36);
    skyboxShaderPro->disableAttributeArray("aPos");
    skybox->release();
    skyboxVBO->release();
    skyboxShaderPro->release();
    QOpenGLContext::currentContext()->functions()->glDepthFunc(GL_LESS);
}

void CSkyBox::setAspect(float _aspect)
{
    aspect=_aspect;
}
