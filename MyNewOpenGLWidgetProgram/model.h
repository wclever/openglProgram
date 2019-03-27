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
#include <QVector3D>
#include <QOpenGLShaderProgram>

using namespace std;

class Model
{
public:
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    Model(string const &path, bool gamma = false);
    void Draw(QOpenGLShaderProgram *program);

private:
    void loadModel(string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh);

};

#endif // MODEL_H
