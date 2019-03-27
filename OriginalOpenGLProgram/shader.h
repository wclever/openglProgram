#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

#include <QOpenGLFunctions_3_3_Core>

class Shader
{
public:
    unsigned int ID;
    ~Shader();
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    void use();
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value);
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value);
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value);
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value);
    void setVec2(const std::string &name, float x, float y);
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value);
    void setVec3(const std::string &name, float x, float y, float z);
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value);
    void setVec4(const std::string &name, float x, float y, float z, float w);
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat);
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat);
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat);

private:
    void checkCompileErrors(GLuint shader, std::string type);
    QOpenGLFunctions_3_3_Core *f;

};

#endif // SHADER_H
