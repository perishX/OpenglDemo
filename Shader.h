#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <QOpenGLFunctions_3_3_Core>

class Shader : public QOpenGLFunctions_3_3_Core{
public:
    Shader();
    Shader(const char* vertexShaderPath,const char* fragmentShaderPath);
    Shader(const char* vertexShaderPath,const char* fragmentShaderPath,const char* geometryShaderPath);
    ~Shader();
    void init(const char* vertexShaderPath,const char* fragmentShaderPath);
    void init(const char* vertexShaderPath,const char* fragmentShaderPath,const char* geometryShaderPath);
    unsigned int getID();
protected:
private:
    unsigned int getVertexShader(const char* vertexShaderPath);
    unsigned int getFragmentShader(const char* fragmentShaderPath);
    unsigned int getGeometryShader(const char* geometryShaderPath);
    unsigned int ID{};
};

#endif // SHADER_H