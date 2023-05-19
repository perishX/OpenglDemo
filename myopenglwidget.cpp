#include "myopenglwidget.h"

const char* vertexShaderSource=
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 perspective;\n"
        "void main()\n"
        "{\n"
        "gl_Position = perspective*view*model*vec4(aPos,1.f);\n"
        "}\n\0";

const char* fragmentShaderSource=
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent):QOpenGLWidget{parent}{
    this->grabKeyboard();
    this->viewer.updateDirection();

}

void MyOpenGLWidget::resizeEvent(QResizeEvent *event){
    //std::cout<<"MyOpenGLWidget: "<<this->width()<<" "<<this->height()<<std::endl;
    this->g_width=this->width();
    this->g_height=this->height();
    //glViewport(0,0,this->g_width,this->g_height);
    //update();
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event){
    //std::cout<<"MyOpenGLWidget: keyPressEvent "<<std::endl;
    if(event->key()==Qt::Key_W){
        this->viewer.setForwardFlag(1);
    }else if(event->key()==Qt::Key_S){
        this->viewer.setForwardFlag(-1);
    }

    if(event->key()==Qt::Key_D){
       this->viewer.setRightFlag(1);
    }else if(event->key()==Qt::Key_A){
        this->viewer.setRightFlag(-1);
    }

    if(event->key()==Qt::Key_Q){
        this->viewer.setUpFlag(1);
    }else if(event->key()==Qt::Key_E){
        this->viewer.setUpFlag(-1);
    }
    this->viewer.move();
    update();
}

void MyOpenGLWidget::keyReleaseEvent(QKeyEvent *event){
    //std::cout<<"keyReleaseEvent"<<std::endl;
    if(event->key()==Qt::Key_W || event->key()==Qt::Key_S){
        this->viewer.setForwardFlag(0);
    }

    if(event->key()==Qt::Key_D || event->key()==Qt::Key_A){
        this->viewer.setRightFlag(0);
    }

    if(event->key()==Qt::Key_Q || event->key()==Qt::Key_E){
        this->viewer.setUpFlag(0);
    }
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
    }
    if(event->button()==Qt::RightButton){
        this->lastX=event->x();
        this->lastY=event->y();
    }
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
    }
    if(event->button()==Qt::RightButton){
    }
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event){
    //std::cout<<"mouseMoveEvent: "<<event->x()<<" "<<event->y()<<std::endl;
    if(event->buttons()&Qt::RightButton){
        float xOffset=event->x()-this->lastX;
        float yOffset=this->lastY-event->y();

        this->lastX=event->x();
        this->lastY=event->y();

        this->viewer.updateDirection(xOffset,yOffset);
        update();
    }

}

void MyOpenGLWidget::wheelEvent(QWheelEvent *event){
    float offset{};
    if(event->delta() > 0){
        offset=-1;
    }else{
        offset=1;
    }
    this->viewer.zoom(offset);
    update();
}

void MyOpenGLWidget::initializeGL(){
    //std::cout<<"MyOpenGLWidget::initializeGL"<<std::endl;
    initializeOpenGLFunctions();
    glGenVertexArrays(1,&this->VAO_id);
    glGenBuffers(1,&this->VBO_id);
    glBindVertexArray(this->VAO_id);
    glBindBuffer(GL_ARRAY_BUFFER,this->VBO_id);
    glBufferData(GL_ARRAY_BUFFER,sizeof(this->vertices),this->vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    this->ID = this->init("C:/Users/73965/Documents/demo/shaders/shader.vert","C:/Users/73965/Documents/demo/shaders/shader.frag");

    glEnable(GL_DEPTH_TEST);
}

void MyOpenGLWidget::resizeGL(int w, int h){
    //std::cout<<"MyOpenGLWidget::resizeGL"<<std::endl;
    //glViewport(0,0,this->g_width,this->g_height);
}

void MyOpenGLWidget::paintGL(){
    //std::cout<<"MyOpenGLWidget::paintGL"<<std::endl;

    glViewport(0,0,this->g_width,this->g_height);

    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model = glm::mat4{1.0f};
    glm::mat4 view = glm::mat4{1.0f};
    view = this->viewer.getViewMatrix();
    glm::mat4 perspective = glm::mat4{1.0f};
    perspective = glm::perspective(glm::radians(this->viewer.getFov()), static_cast<float>(this->g_width) / static_cast<float>(this->g_height), 0.1f, 100.0f);

    //glUseProgram(this->shader_id);
    glUseProgram(this->ID);
    glUniformMatrix4fv(glGetUniformLocation(this->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(this->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(this->ID, "perspective"), 1, GL_FALSE, glm::value_ptr(perspective));

    glBindVertexArray(this->VAO_id);
    glDrawArrays(GL_TRIANGLES,0,36);

}


unsigned int MyOpenGLWidget::init(const char* vertexShaderPath,const char* fragmentShaderPath){
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

    return shaderProgram;
}

unsigned int MyOpenGLWidget::init(const char* vertexShaderPath,const char* fragmentShaderPath,const char* geometryShaderPath){
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

    return shaderProgram;
}

unsigned int MyOpenGLWidget::getVertexShader(const char *vertexShaderPath){
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

unsigned int MyOpenGLWidget::getFragmentShader(const char *fragmentShaderPath){
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

unsigned int MyOpenGLWidget::getGeometryShader(const char *geometryShaderPath){
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
        std::cerr << "ERROR::SHADER::Geometry::COMPILATION_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "Geometry SHADER COMPILE SUCCESS" << '\n';
    }
    return shaderID;
}

