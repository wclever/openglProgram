#include "model.h"


Model::Model(const string &path, bool gamma) : gammaCorrection(gamma)
{
    MaxPos = glm::vec3(0.0f, 0.0f, 0.0f);
    MinPos = glm::vec3(0.0f, 0.0f, 0.0f);
    MiddlePos = glm::vec3(0.0f, 0.0f, 0.0f);
    MovePos = glm::vec3(0.0f, 0.0f, 0.0f);
    MIN = 0;

    loadModel(path);
}

void Model::Draw(Shader* shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].drawMesh(shader);
    }
}

void Model::Draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].drawMesh();
    }
}

void Model::loadModel(const string &path)
{
    Assimp::Importer importer;
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
        aiComponent_CAMERAS
        | aiComponent_LIGHTS
        | aiComponent_TANGENTS_AND_BITANGENTS
        //| aiComponent_MATERIALS
        //| aiComponent_TEXTURES
        | aiComponent_BONEWEIGHTS
        | aiComponent_ANIMATIONS
        | aiComponent_NORMALS
    );
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_GenSmoothNormals |
        aiProcess_RemoveComponent |
        aiProcess_FixInfacingNormals |
        aiProcess_ImproveCacheLocality |
        aiProcess_OptimizeMeshes
    );
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


    MiddlePos.x = 1.0f / (MaxPos.x - MinPos.x);
    MiddlePos.y = 1.0f / (MaxPos.y - MinPos.y);
    MiddlePos.z = 2.0f / (MaxPos.z - MinPos.z);
    MIN = MiddlePos.x;
    if (MiddlePos.y < MIN) MIN = MiddlePos.y;
    if (MiddlePos.z < MIN) MIN = MiddlePos.z;
    //cout << "缩放比例: " << MIN << endl;

    MovePos.x = -(MaxPos.x + MinPos.x) / 2.0f * MIN;
    MovePos.y = -(MaxPos.y + MinPos.y) / 2.0f * MIN;
    MovePos.z = -(MaxPos.z + MinPos.z) / 2.0f * MIN;

    cout << "MovePos.x:" << MovePos.x << "\tMovePos.y:" << MovePos.y << "\tMovePos.z:" << MovePos.z << endl;
    cout << "meshes size: " << meshes.size() << endl << endl;
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

        if (MaxPos == glm::vec3(0.0f) && MinPos == glm::vec3(0.0f))
        {
            MaxPos = temp_vector;
            MinPos = temp_vector;
        }
        if (temp_vector.x > MaxPos.x) MaxPos.x = temp_vector.x;
        if (temp_vector.y > MaxPos.y) MaxPos.y = temp_vector.y;
        if (temp_vector.z > MaxPos.z) MaxPos.z = temp_vector.z;
        if (temp_vector.x < MinPos.x) MinPos.x = temp_vector.x;
        if (temp_vector.y < MinPos.y) MinPos.y = temp_vector.y;
        if (temp_vector.z < MinPos.z) MinPos.z = temp_vector.z;

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
    cout << "MaxPos.x:" << MaxPos.x << "\tMaxPos.y:" << MaxPos.y << "\tMaxPos.z:" << MaxPos.z << endl;
    cout << "MinPos.x:" << MinPos.x << "\tMinPos.y:" << MinPos.y << "\tMinPos.z:" << MinPos.z << endl;

    return Mesh(vertices, indices);
}
