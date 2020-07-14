#include "model.h"

Model::Model(const string &path, bool gamma) : gammaCorrection(gamma)
{
    MaxPos = glm::vec3(0.0f, 0.0f, 0.0f);
    MinPos = glm::vec3(0.0f, 0.0f, 0.0f);
    MiddlePos = glm::vec3(0.0f, 0.0f, 0.0f);
    MovePos = glm::vec3(0.0f, 0.0f, 0.0f);
    MIN = 0;
    index_flag = 0;
    flag = false;

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
        | aiComponent_MATERIALS
        | aiComponent_TEXTURES
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
        aiProcess_OptimizeMeshes |
        aiProcess_FindDegenerates |
//        aiProcess_PreTransformVertices |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_FindInvalidData
    );
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);

    //检查是否法向量错误
    if(!temp_vertice.empty()){
        glm::vec3 temp_normal = glm::vec3(0.0);
        for(vector<Vertex>::iterator it = temp_vertice.begin(); it != temp_vertice.end(); it++){
            temp_normal += it->Normal;
        }
        temp_normal = temp_normal / glm::vec3(temp_vertice.size());
        if(temp_normal.y < 0) flag = true;
        //qDebug() << temp_vertice.size();
        //qDebug() << temp_vertice[0].Normal.x <<temp_vertice[0].Normal.y<<temp_vertice[0].Normal.z;
        //qDebug() << temp_vertice[1].Normal.x <<temp_vertice[1].Normal.y<<temp_vertice[1].Normal.z;
    }
    if(flag){
        for(auto& val:full_vertice){
            val.Normal =- val.Normal;
        }
    }

    front_vertice = full_vertice;
    //排序所有顶点以便于顶点匹配
    sort(full_vertice.begin(), full_vertice.end(), cmp);

//    for(vector<Vertex>::iterator it = full_vertice.begin(); it != full_vertice.end(); ++it){
//        qDebug() << it->Position.x << it->Position.y << it->Position.z << it->Normal.x << it->Normal.y << it->Normal.z;
//    }

    int flag = 0;
    bool first_test = true;
    for(vector<Vertex>::iterator it = full_vertice.begin(); it != full_vertice.end(); ++it){
        if(first_test){
            temp_pos = it->Position;
            temp_normal = glm::vec3(0.0);
            first_test = false;
        }
        if(fabs(it->Position.x - temp_pos.x) < DBL_EPSILON && fabs(it->Position.y - temp_pos.y) < DBL_EPSILON
                && fabs(it->Position.z - temp_pos.z) < DBL_EPSILON){
            flag++;
            temp_normal += glm::normalize(it->Normal);
        }
        else {
            temp_normal.x = temp_normal.x / flag;
            temp_normal.y = temp_normal.y / flag;
            temp_normal.z = temp_normal.z / flag;

            Vertex temp;
            temp.Position = temp_pos;
            temp.Normal = temp_normal;
            only_vertice.push_back(temp);

            temp_pos = it->Position;
            temp_normal = it->Normal;
            flag = 1;
        }
    }
    temp_normal.x = temp_normal.x / flag;
    temp_normal.y = temp_normal.y / flag;
    temp_normal.z = temp_normal.z / flag;
    //顶点法向量
    Vertex temp;
    temp.Position = temp_pos;
    temp.Normal = temp_normal;
    only_vertice.push_back(temp);

    //邻接矩阵计算
    auto vertice_length = only_vertice.size();
    ad_matrix.resize(vertice_length);
    face3d_matrix.resize(vertice_length);
    unsigned int position_index_0 = 0;
    unsigned int position_index_1 = 0;
    unsigned int position_index_2 = 0;
    unsigned int face_index = 0;
    unsigned int face3d_index = 0;
    tri_face triangle_face;
    vector<tri_face> face_vec;//三角面片的集合
    //遍历front所有三角形，然后从only中找到顶点索引，然后标记这个点的索引加入ad_matrix
    for(vector<Vertex>::iterator it = front_vertice.begin(); it != front_vertice.end(); ++it){  
        //遍历only查找索引，从0开始
        for(vector<Vertex>::iterator itt = only_vertice.begin(); itt != only_vertice.end(); ++itt){
            if(it->Position == itt->Position){
                if(face_index == 0){
                    position_index_0 = distance(only_vertice.begin(),itt);//返回第几个元素
                    triangle_face.vertex_index0 = position_index_0;
                    face_index++;
                    break;
                }
                else if(face_index == 1){
                    position_index_1 = distance(only_vertice.begin(),itt);//返回第几个元素
                    triangle_face.vertex_index1 = position_index_1;
                    face_index++;
                    break;
                }
                else if(face_index == 2){
                    position_index_2 = distance(only_vertice.begin(),itt);//返回第几个元素
                    triangle_face.vertex_index2 = position_index_2;
                    face_index = 0;//if(position_index_2 < 10) qDebug() << position_index_2;
                    face_vec.push_back(triangle_face);
                    face3d_index++;
                    break;
                }
                else {
                    qDebug() << "索引无法找到";
                    break;
                }
            }

        }
        //如果第一次添加顶点
        if(face_index == 0){//qDebug() << "tianjia";
            if(ad_matrix.size() == vertice_length){
                ad_matrix[position_index_0].push_back(position_index_1);
                ad_matrix[position_index_0].push_back(position_index_2);
                ad_matrix[position_index_1].push_back(position_index_0);
                ad_matrix[position_index_1].push_back(position_index_2);
                ad_matrix[position_index_2].push_back(position_index_0);
                ad_matrix[position_index_2].push_back(position_index_1);
                face3d_matrix[position_index_0].push_back(face3d_index-1);
                face3d_matrix[position_index_1].push_back(face3d_index-1);
                face3d_matrix[position_index_2].push_back(face3d_index-1);
            }
            else{
                //检索每个索引是否有重复的点，没有就加入新的点
                vector<unsigned int>::iterator ret;

                ret = find(ad_matrix[position_index_0].begin(), ad_matrix[position_index_0].end(), position_index_1);
                if(ret == ad_matrix[position_index_0].end()){
                    ad_matrix[position_index_0].push_back(position_index_1);
                }
                ret = find(ad_matrix[position_index_0].begin(), ad_matrix[position_index_0].end(), position_index_2);
                if(ret == ad_matrix[position_index_0].end()){
                    ad_matrix[position_index_0].push_back(position_index_2);
                }

                ret = find(ad_matrix[position_index_1].begin(), ad_matrix[position_index_1].end(), position_index_0);
                if(ret == ad_matrix[position_index_1].end()){
                    ad_matrix[position_index_1].push_back(position_index_0);
                }
                ret = find(ad_matrix[position_index_1].begin(), ad_matrix[position_index_1].end(), position_index_2);
                if(ret == ad_matrix[position_index_1].end()){
                    ad_matrix[position_index_1].push_back(position_index_2);
                }

                ret = find(ad_matrix[position_index_2].begin(), ad_matrix[position_index_2].end(), position_index_0);
                if(ret == ad_matrix[position_index_2].end()){
                    ad_matrix[position_index_2].push_back(position_index_0);
                }
                ret = find(ad_matrix[position_index_2].begin(), ad_matrix[position_index_2].end(), position_index_1);
                if(ret == ad_matrix[position_index_2].end()){
                    ad_matrix[position_index_2].push_back(position_index_1);
                }

                //加入三角面矩阵
                ret = find(face3d_matrix[position_index_0].begin(), face3d_matrix[position_index_0].end(), face3d_index-1);
                if(ret == face3d_matrix[position_index_0].end()){
                    face3d_matrix[position_index_0].push_back(face3d_index-1);
                }
                ret = find(face3d_matrix[position_index_1].begin(), face3d_matrix[position_index_1].end(), face3d_index-1);
                if(ret == face3d_matrix[position_index_1].end()){
                    face3d_matrix[position_index_1].push_back(face3d_index-1);
                }
                ret = find(face3d_matrix[position_index_2].begin(), face3d_matrix[position_index_2].end(), face3d_index-1);
                if(ret == face3d_matrix[position_index_2].end()){
                    face3d_matrix[position_index_2].push_back(face3d_index-1);
                }
            }

        }

    }

    //区域划分
    double focus_depth = 2.0;//焦深
    double range = 50.0;//直径
    double temp_focus_depth = 0.0;
    double temp_range = 0.0;
    unsigned int vertice_size = only_vertice.size();//顶点数量qDebug() << vertice_size;
    vector<unsigned int> vertice_flag(vertice_size);//是否取完所有顶点的标记；
    unsigned int face_size = face_vec.size();//面片的数量
    vector<unsigned int> face_flag(face_size);//面片的取完标记
    vector<unsigned int> vertex_index;

    //点方式计算
    //取第一个未加入的点加入划分的范围
//    vector<unsigned int>::iterator it = find(vertice_flag.begin(), vertice_flag.end(), 0);
//    if(it != vertice_flag.end()){
//        unsigned int first_num = distance(vertice_flag.begin(), it);
//        vertex_index.push_back(first_num);
//        vertice_flag[first_num] = 1;
//        //加入邻接点
//        for(vector<unsigned int>::iterator vec = vertex_index.begin(); vec != vertex_index.end(); vec++){
//            //扩展邻接点范围
//            unsigned int index_num = *vec;
//            for(vector<unsigned int>::iterator it = ad_matrix[index_num].begin(); it != ad_matrix[index_num].end(); ++it){

//                for(vector<unsigned int>::iterator itt = vertex_index.begin(); itt != vertex_index.end(); itt++){
//                    //加入邻接矩阵邻接点
//                    unsigned int first_index = *itt;
//                    unsigned int second_index = *it;
//                    double middle_range = sqrt(pow(only_vertice[second_index].Position.x - only_vertice[first_index].Position.x, 2.0)
//                            + pow(only_vertice[second_index].Position.y - only_vertice[first_index].Position.y, 2.0)
//                            + pow(only_vertice[second_index].Position.z - only_vertice[first_index].Position.z, 2.0));
//                    if(middle_range - range < 0){
//                        if(middle_range > temp_range) temp_range = middle_range;
//                        //计算焦深，如果焦深在范围内加入这个点
//                        //计算合成法向量并单位化
//                        glm::vec3 com_normal = glm::vec3(0.0f);
//                        com_normal.x = only_vertice[first_index].Normal.x + only_vertice[second_index].Normal.x;
//                        com_normal.y = only_vertice[first_index].Normal.y + only_vertice[second_index].Normal.y;
//                        com_normal.z = only_vertice[first_index].Normal.z + only_vertice[second_index].Normal.z;
//                        com_normal = glm::normalize(com_normal);
//                        //计算焦深d=|A(x1-x2)+B(y1-y2)+C(z1-z2)|
//                        double middle_focus_depth = fabs(com_normal.x * (only_vertice[first_index].Position.x - only_vertice[second_index].Position.x) +
//                                                com_normal.y * (only_vertice[first_index].Position.y - only_vertice[second_index].Position.y) +
//                                                com_normal.z * (only_vertice[first_index].Position.z - only_vertice[second_index].Position.z));
//                        //判断焦深是否在范围内
//                        if(middle_focus_depth < focus_depth){
//                            if(middle_focus_depth > temp_focus_depth) temp_focus_depth = middle_focus_depth;
//                            if(vertice_flag[second_index] == 0){
//                                vertex_index.push_back(second_index);
//                                vertice_flag[second_index] = 1;
//                            }

//                        }//完成加入一个点
//                    }
//                }
//            }
//        }

//    }

    //面方式计算
    vector<unsigned int> region_partition;
    vector<unsigned int> region_partition_num;
    //取第一个三角形的第一个点加入集合
    for(vector<tri_face>::iterator face_it = face_vec.begin(); face_it != face_vec.end(); ++face_it){
        unsigned int face_num = 0;
    }

//    for(vector<Vertex>::iterator it = only_vertice.begin(); it != only_vertice.end(); ++it){
//        qDebug() << it->Position.x << it->Position.y << it->Position.z << it->Normal.x << it->Normal.y << it->Normal.z;
//    }

    //qDebug() << "meshes size: " << meshes.size();
    //qDebug() << "MaxPos.x:" << MaxPos.x << "\tMaxPos.y:" << MaxPos.y << "\tMaxPos.z:" << MaxPos.z;
    //qDebug() << "MinPos.x:" << MinPos.x << "\tMinPos.y:" << MinPos.y << "\tMinPos.z:" << MinPos.z;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh));


        //标记第i个mesh的index
        if(flag)
        {
            index_flag = uint(meshes.size());
            Vertex temp_vertex;
            for(vector<unsigned int>::iterator it = index.begin(); it != index.end(); it++){
                temp_vertex = meshes[index_flag-1].vertices[*it];
                if(temp_vertice.empty()){
                    temp_vertice.push_back(temp_vertex);
                }
                else {
                    if(temp_vertice[0].Position == temp_vertex.Position){
                        temp_vertice.push_back(temp_vertex);
                    }
                    else{
                        temp_vertice.clear();
                        temp_vertice.push_back(temp_vertex);
                    }
                }
            }
            index.clear();
            flag = false;
        }
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

//    qDebug() << "meshes size: " << meshes.size();
//    qDebug() << "MaxPos.x:" << MaxPos.x << "\tMaxPos.y:" << MaxPos.y << "\tMaxPos.z:" << MaxPos.z;
//    qDebug() << "MinPos.x:" << MinPos.x << "\tMinPos.y:" << MinPos.y << "\tMinPos.z:" << MinPos.z;

}

Mesh Model::processMesh(aiMesh *mesh)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    normal_index.clear();

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 temp_vector = glm::vec3(0.0f);

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
        if (temp_vector.y >= MaxPos.y) {MaxPos.y = temp_vector.y; index.push_back(i); flag = true;}
        if (temp_vector.z > MaxPos.z) MaxPos.z = temp_vector.z;
        if (temp_vector.x < MinPos.x) MinPos.x = temp_vector.x;
        if (temp_vector.y < MinPos.y) MinPos.y = temp_vector.y;
        if (temp_vector.z < MinPos.z) MinPos.z = temp_vector.z;

        temp_vector = glm::vec3(0.0f);
        if(mesh->HasNormals())
        {
            temp_vector.x = mesh->mNormals[i].x;
            temp_vector.y = mesh->mNormals[i].y;
            temp_vector.z = mesh->mNormals[i].z;
        }
        else
        {
            qDebug() << i;
            normal_index.push_back(i);
        }
        vertex.Normal = temp_vector;

        vertices.push_back(vertex);
        full_vertice.push_back(vertex);
    }

    //计算法向量缺失
    glm::vec3 normal = glm::vec3(0.0f);
    for(unsigned int i = 0, vn = 1; i < normal_index.size(); i++)
    {
        unsigned int m = normal_index[i];
        qDebug() << vertices[i].Position.x << vertices[i].Position.y << vertices[i].Position.z;
        if(vn)
        {
            glm::vec3 va = vertices[1].Position - vertices[0].Position;
            glm::vec3 vb = vertices[2].Position - vertices[0].Position;
            glm::vec3 norm = glm::cross(va, vb);
            normal = glm::normalize(norm);
            //qDebug() << va.x << va.y << va.z;
            //qDebug() << vb.x << vb.y << vb.z;
            vn = 0;
        }

        vertices[m].Normal = normal;
    }
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices);
}

bool Model::cmp(const Vertex &a, const Vertex &b)
{
    if (fabs(a.Position.x - b.Position.x) < DBL_EPSILON){
        if(fabs(a.Position.y - b.Position.y) < DBL_EPSILON)
            return a.Position.z < b.Position.z;
        else
            return a.Position.y < b.Position.y;
    }
    else
        return a.Position.x < b.Position.x;

}
