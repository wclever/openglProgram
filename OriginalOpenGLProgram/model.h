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

    Model(string const &path, bool gamma = false);
    void Draw(Shader* shader);
    void Draw();

private:
    void loadModel(string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh);

};

#endif // MODEL_H
