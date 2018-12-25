#include "cmodel.h"

CModel::CModel(QString _name, bool gamma,QObject *parent):
    QObject(parent), gammaCorrection(gamma)
{
    loadModel(_name);
    aspect=1.0;
    shaderProgram=new QOpenGLShaderProgram(this);
    model.setToIdentity();
}

void CModel::loadModel(QString _name)
{
    directory=_name;
    Importer importer;
    const aiScene *scene=importer.ReadFile(_name.toStdString(),aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString();
        return;
    }
    processNode(scene->mRootNode, scene);



}

void CModel::initModel()
{
    QOpenGLShader *vShader=new QOpenGLShader(QOpenGLShader::Vertex);
    qDebug()<<"Compile Model Vertex Shader"<<vShader->compileSourceFile(":/model.vert");
    QOpenGLShader *fShader=new QOpenGLShader(QOpenGLShader::Fragment);
    qDebug()<<"Compile Model Fragment Shader"<<fShader->compileSourceFile(":/model.frag");
    shaderProgram->create();
    shaderProgram->addShader(vShader);
    shaderProgram->addShader(fShader);
    shaderProgram->link();
    for(int i = 0; i < meshes.size(); i++)
        meshes[i].setupMesh();
}

void CModel::draw(QMatrix4x4 _camera,QVector3D _cam_pos)
{
    shaderProgram->bind();
    QMatrix4x4 projection;
    projection.perspective(45.0,aspect,0.005,5000.0);
    shaderProgram->setUniformValue("model",model);
        shaderProgram->setUniformValue("invmodel",model.normalMatrix());
    shaderProgram->setUniformValue("view",_camera);
    shaderProgram->setUniformValue("projection",projection);

    shaderProgram->setUniformValue("light.ambient", QVector3D(0.2f, 0.2f, 0.2f));
    shaderProgram->setUniformValue("light.diffuse", QVector3D(0.9f, 0.9f, 0.9f));
    shaderProgram->setUniformValue("light.specular", QVector3D(1.0f, 1.0f, 1.0f));
    shaderProgram->setUniformValue("light.position", QVector3D(1.0f, -1.0f, 1.0f));
    shaderProgram->setUniformValue("viewPos", _cam_pos);
    for(int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shaderProgram);
    shaderProgram->release();
}
void CModel::processNode(aiNode *node, const aiScene *scene)
{
//    aiMatrix4x4 mat=node->mTransformation;
//    qDebug()<<mat.a1<<mat.b1<<mat.c1<<mat.d1;
//    qDebug()<<mat.a2<<mat.b2<<mat.c2<<mat.d2;
//    qDebug()<<mat.a3<<mat.b3<<mat.c3<<mat.d3;
//    qDebug()<<mat.a4<<mat.b4<<mat.c4<<mat.d4;

    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

CMesh CModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    Texture textures;

    // Walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        QVector3D vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.Position = vector;
        // normals
        vector.setX(mesh->mNormals[i].x);
        vector.setY(mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);
        vertex.Normal = vector;
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            QVector2D vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.setX( mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = QVector2D(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    aiColor4D diffuse;
    aiColor4D specular;
    aiColor4D ambient;
    QVector3D t_diffuse3d(1.0f, 0.5f, 0.31f);
    QVector3D t_specular3d(1.0f, 0.5f, 0.31f);
    QVector3D t_ambient3d(1.0f, 0.5f, 0.31f);
    float t_shininess=32.0;
    if(AI_SUCCESS ==aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
        t_diffuse3d=QVector3D(diffuse.r,diffuse.g,diffuse.b);
    if(AI_SUCCESS ==aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular))
        t_specular3d=QVector3D(specular.r,specular.g,specular.b);
    if(AI_SUCCESS ==aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)  )
        t_ambient3d=QVector3D(ambient.r,ambient.g,ambient.b);
    float shininess;
    if(AI_SUCCESS ==aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess))
        t_shininess=shininess;
    textures.ambient=t_ambient3d;
    textures.diffuse=t_diffuse3d;
    textures.specular=t_specular3d;
    textures.shininess=t_shininess;

    if(material->GetTextureCount(aiTextureType_DIFFUSE)>0){
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        textures.diffuse_path=m_format_path(QString(str.C_Str()));
    }
    else
        textures.diffuse_path.clear();
    if(material->GetTextureCount(aiTextureType_SPECULAR)>0){
        aiString str;
        material->GetTexture(aiTextureType_SPECULAR, 0, &str);
        textures.specular_path=m_format_path(QString(str.C_Str()));
    }
    else
        textures.specular_path.clear();
    if(material->GetTextureCount(aiTextureType_EMISSIVE)>0){
        aiString str;
        material->GetTexture(aiTextureType_EMISSIVE, 0, &str);
        textures.emission_path=m_format_path(QString(str.C_Str()));
    }
    else
        textures.emission_path.clear();
    //    aiColor4D diffuse;
    //    aiColor4D specular;
    //    aiColor4D ambient;
    //    ai_real shininess=0.0;
    //    if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)){
    //        textures.diffuse=QVector3D(diffuse.r,diffuse.g,diffuse.b);
    //    }
    //    if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)){
    //        textures.specular=QVector3D(specular.r,specular.g,specular.b);
    //    }
    //    if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)){
    //        textures.ambient=QVector3D(ambient.r,ambient.g,ambient.b);
    //    }
    //    if(AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess)){
    //        textures.shininess=shininess/4.0;
    //    }
    //    qDebug()<<textures.shininess<<shininess;
    return CMesh(vertices, indices, textures);
}

QString CModel::m_format_path(QString filename){
    QString result;
    result.clear();
    QString t_dir=directory;
    t_dir=t_dir.replace(QRegExp(QString("\\\\")),QString("/"));

    t_dir=t_dir.left(t_dir.lastIndexOf(QChar('/')))+tr("/");
    QFile t_file;
    t_file.setFileName(filename);
    if(t_file.exists()){
        result= filename;
    }
    else{
        QString t_name_2=t_dir+filename;
        t_file.setFileName(t_name_2);
        if(t_file.exists())
            result= t_name_2;
        else{
            QString t_name_3=filename;
            t_name_3=t_name_3.replace(QRegExp(QString("\\\\")),QString("/"));
            t_name_3=t_name_3.replace(QRegExp(".+/"),QString());
            t_name_3=t_dir+t_name_3;
            t_file.setFileName(t_name_3);
            if(t_file.exists())
                result= t_name_3;
        }
    }
    qDebug()<<result<<"NAME";
    return result;
}

void CModel::setRotateX(float angle)
{
    model.rotate(angle,1.0,0.0,0.0);
}

void CModel::setRotateY(float angle)
{
    model.rotate(angle,0.0,1.0,0.0);
}

void CModel::setRotateZ(float angle)
{
    model.rotate(angle,0.0,0.0,1.0);
}

void CModel::setAspect(float _aspect)
{
    aspect=_aspect;
}

void CModel::setPosition(float _x, float _y, float _z)
{
    model.setColumn(3,QVector4D(_x,_y,_z,1.0));
}

float CModel::x()
{
    return model(0,3);
}

float CModel::y()
{
    return model(1,3);
}

float CModel::z()
{
    return model(2,3);
}

void CModel::set_x(float _x)
{
    model(0,3)=_x;
}

void CModel::set_y(float _y)
{
    model(1,3)=_y;
}

void CModel::set_z(float _z)
{
    model(2,3)=_z;
}

QMatrix4x4 CModel::getMatrix()
{
    return model;
}

void CModel::setMatrix(QMatrix4x4 _model)
{
    model=_model;
}
