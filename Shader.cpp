#include "Shader.h"

Shader::Shader(){

}

Shader::Shader(QOpenGLWidget* parent){
    this->parent=parent;
}

Shader::Shader(const char* vertexShaderPath,const char* fragmentShaderPath){
//    this->init(vertexShaderPath,fragmentShaderPath);
}
Shader::Shader(const char* vertexShaderPath,const char* fragmentShaderPath,const char* geometryShaderPath){
    this->init(vertexShaderPath,fragmentShaderPath,geometryShaderPath);
}
Shader::~Shader(){

}

unsigned int Shader::getID(){
    return this->ID;
}

void Shader::init(QOpenGLWidget* widget,const char* vertexShaderPath,const char* fragmentShaderPath){
    //QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();
    initializeOpenGLFunctions();
    unsigned int vertexShader=this->getVertexShader(vertexShaderPath);
    unsigned int fragmentShader=this->getFragmentShader(fragmentShaderPath);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char info[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, info);
        std::cerr << "ERROR::LINK_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "LINK SUCCESS" << '\n';
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    this->ID = shaderProgram;
}

void Shader::init(const char* vertexShaderPath,const char* fragmentShaderPath,const char* geometryShaderPath){
    //initializeOpenGLFunctions();
    this->parent->makeCurrent();
    unsigned int vertexShader=this->getVertexShader(vertexShaderPath);
    unsigned int fragmentShader=this->getFragmentShader(fragmentShaderPath);
    unsigned int geometryShader=this->getGeometryShader(geometryShaderPath);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, geometryShader);
    glLinkProgram(shaderProgram);

    int success;
    char info[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, info);
        std::cerr << "ERROR::LINK_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "LINK SUCCESS" << '\n';
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);

    this->ID = shaderProgram;
}

unsigned int Shader::getVertexShader(const char *vertexShaderPath){
    // 1. 从文件路径中获取顶点/片段着色器
    std::string source;
    std::ifstream shaderFile;
    // 保证ifstream对象可以抛出异常：
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开文件
        shaderFile.open(vertexShaderPath);
        std::stringstream shaderStream;
        // 读取文件的缓冲内容到数据流中
        shaderStream << shaderFile.rdbuf();
        // 关闭文件处理器
        shaderFile.close();
        // 转换数据流到string
        source = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char *shaderSource = source.c_str();
    int success;
    char info[512];
    unsigned int shaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderID, 1, &shaderSource, nullptr);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, nullptr, info);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "VERTEX SHADER COMPILE SUCCESS" << '\n';
    }
    return shaderID;
}

unsigned int Shader::getFragmentShader(const char *fragmentShaderPath){
    // 1. 从文件路径中获取顶点/片段着色器
    std::string source;
    std::ifstream shaderFile;
    // 保证ifstream对象可以抛出异常：
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开文件
        shaderFile.open(fragmentShaderPath);
        std::stringstream shaderStream;
        // 读取文件的缓冲内容到数据流中
        shaderStream << shaderFile.rdbuf();
        // 关闭文件处理器
        shaderFile.close();
        // 转换数据流到string
        source = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout<<fragmentShaderPath<<std::endl;
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char *shaderSource = source.c_str();

    int success;
    char info[512];

    unsigned int shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderID, 1, &shaderSource, nullptr);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, nullptr, info);
        std::cerr << "ERROR::SHADER::Fragment::COMPILATION_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "Fragment SHADER COMPILE SUCCESS" << '\n';
    }
    return shaderID;
}

unsigned int Shader::getGeometryShader(const char *geometryShaderPath){
    // 1. 从文件路径中获取顶点/片段着色器
    std::string source;
    std::ifstream shaderFile;
    // 保证ifstream对象可以抛出异常：
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开文件
        shaderFile.open(geometryShaderPath);
        std::stringstream shaderStream;
        // 读取文件的缓冲内容到数据流中
        shaderStream << shaderFile.rdbuf();
        // 关闭文件处理器
        shaderFile.close();
        // 转换数据流到string
        source = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char *shaderSource = source.c_str();

    int success;
    char info[512];

    unsigned int shaderID = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(shaderID, 1, &shaderSource, nullptr);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, nullptr, info);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "VERTEX SHADER COMPILE SUCCESS" << '\n';
    }
    return shaderID;
}


void Shader::setInt(const char *name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name), value);
}
void Shader::setBool(const char * name,bool value){
    glUniform1i(glGetUniformLocation(ID, name), value);
}
void Shader::setFloat(const char *name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name), value);
}
void Shader::setVector3f(const char *name, int count, const float *value)
{
    glUniform3fv(glGetUniformLocation(ID, name), count, value);
}
void Shader::setVector2f(const char *name, int count, const float *value)
{
    glUniform2fv(glGetUniformLocation(ID, name), count, value);
}
void Shader::setMatrix4f(const char *name, int count, const float *value)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name), count, GL_FALSE, value);
}
