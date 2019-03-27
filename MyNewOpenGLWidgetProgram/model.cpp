#include "model.h"


Model::Model(const string &path, bool gamma) : gammaCorrection(gamma)
{
    loadModel(path);
}

void Model::Draw(QOpenGLShaderProgram *program)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {   //int xyz = meshes.size();
        meshes[i].drawMesh(program);
    }
}

void Model::loadModel(const string &path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 temp_vector;

        temp_vector.x = mesh->mVertices[i].x;
        temp_vector.y = mesh->mVertices[i].y;
        temp_vector.z = mesh->mVertices[i].z;
        vertex.Position = temp_vector;

        temp_vector.x = mesh->mNormals[i].x;
        temp_vector.y = mesh->mNormals[i].y;
        temp_vector.z = mesh->mNormals[i].z;
        vertex.Normal = temp_vector;

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices);
}
