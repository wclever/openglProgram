#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <string>
#include <iostream>
#include <vector>
#include <shader.h>
#include <QDebug>
#include <algorithm>

using namespace std;

class Model
{
public:
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;
    glm::vec3 MaxPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 MinPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 MiddlePos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 MovePos = glm::vec3(0.0f, 0.0f, 0.0f);
    float MIN;
    vector<unsigned int> index;//mesh中最大y的索引标记
    unsigned int index_flag;//第i个mesh标记
    vector<unsigned int> normal_index;//缺失法向量索引标记
    bool flag;
    vector<Vertex> temp_vertice;
    vector<Vertex> front_vertice;//排序前的顶点
    vector<Vertex> full_vertice;//排序后的顶点
    vector<Vertex> only_vertice;//去重顶点
    vector<vector<unsigned int>> ad_matrix;//邻接点序列矩阵
    vector<vector<unsigned int>> face3d_matrix;//邻接三角形矩阵
    glm::vec3 temp_pos;
    glm::vec3 temp_normal;


    Model(string const &path, bool gamma = false);
    void Draw(Shader* shader);
    void Draw();

private:
    void loadModel(string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh);

    static bool cmp(const Vertex &a, const Vertex &b);

};

#endif // MODEL_H
