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
    this->updateDirection(0,0);
}

void MyOpenGLWidget::resizeEvent(QResizeEvent *event){
    std::cout<<"MyOpenGLWidget: "<<this->width()<<" "<<this->height()<<std::endl;
    this->g_width=this->width();
    this->g_height=this->height();
    //glViewport(0,0,this->g_width,this->g_height);
    //update();
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event){
    //std::cout<<"MyOpenGLWidget: keyPressEvent "<<std::endl;
    if(event->key()==Qt::Key_W){
        std::cout<<"w"<<std::endl;
        this->forwardFlag=1;
    }else if(event->key()==Qt::Key_S){
        std::cout<<"s"<<std::endl;
        this->forwardFlag=-1;
    }

    if(event->key()==Qt::Key_D){
        std::cout<<"d"<<std::endl;
        this->rightFlag=1;
    }else if(event->key()==Qt::Key_A){
        std::cout<<"a"<<std::endl;
        this->rightFlag=-1;
    }

    if(event->key()==Qt::Key_Q){
        std::cout<<"q"<<std::endl;
        this->upFlag=1;
    }else if(event->key()==Qt::Key_E){
        std::cout<<"e"<<std::endl;
        this->upFlag=-1;
    }
    this->Pos += (this->Front * this->forwardFlag + this->Right * this->rightFlag + this->WorldUp * this->upFlag) * 0.1f;
    update();
}

void MyOpenGLWidget::keyReleaseEvent(QKeyEvent *event){
    std::cout<<"keyReleaseEvent"<<std::endl;
    if(event->key()==Qt::Key_W || event->key()==Qt::Key_S){
        this->forwardFlag=0;
    }

    if(event->key()==Qt::Key_D || event->key()==Qt::Key_A){
        this->rightFlag=0;
    }

    if(event->key()==Qt::Key_Q || event->key()==Qt::Key_E){
        this->upFlag=0;
    }
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        std::cout<<"LeftButton Press"<<std::endl;
    }
    if(event->button()==Qt::RightButton){
        std::cout<<"RightButton Press"<<std::endl;
        this->lastX=event->x();
        this->lastY=event->y();
    }
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        std::cout<<"LeftButton Release"<<std::endl;
    }
    if(event->button()==Qt::RightButton){
        std::cout<<"RightButton Release"<<std::endl;
    }
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event){
    std::cout<<"mouseMoveEvent: "<<event->x()<<" "<<event->y()<<std::endl;
    if(event->buttons()&Qt::RightButton){
        float xOffset=event->x()-this->lastX;
        float yOffset=this->lastY-event->y();

        this->lastX=event->x();
        this->lastY=event->y();

        this->updateDirection(xOffset,yOffset);
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
    if (this->fov >= 1.f && this->fov <= 45.f){
        this->fov += offset;
    }
    if (this->fov < 1.f){
        this->fov = 1.f;
    }
    if (this->fov > 45.f){
        this->fov = 45.f;
    }
    update();
}

void MyOpenGLWidget::updateDirection(float yawOffset, float pitchOffset){
    this->yaw += yawOffset*0.1;
    this->pitch += pitchOffset *0.1;

    glm::vec3 front{};
    front.y = sin(glm::radians(pitch));
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    this->Front = glm::normalize(front);

    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Up = glm::normalize(glm::cross(this->Right,this->Front));

}

void MyOpenGLWidget::initializeGL(){
    std::cout<<"MyOpenGLWidget::initializeGL"<<std::endl;
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

    unsigned int vertexShaderID=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID,1,&vertexShaderSource,nullptr);
    glCompileShader(vertexShaderID);

    unsigned int fragmentShaderID=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID,1,&fragmentShaderSource,nullptr);
    glCompileShader(fragmentShaderID);

    this->shader_id=glCreateProgram();
    glAttachShader(this->shader_id,vertexShaderID);
    glAttachShader(this->shader_id,fragmentShaderID);
    glLinkProgram(this->shader_id);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    glEnable(GL_DEPTH_TEST);
}

void MyOpenGLWidget::resizeGL(int w, int h){
    std::cout<<"MyOpenGLWidget::resizeGL"<<std::endl;
    //glViewport(0,0,this->g_width,this->g_height);
}

void MyOpenGLWidget::paintGL(){
    std::cout<<"MyOpenGLWidget::paintGL"<<std::endl;

    glViewport(0,0,this->g_width,this->g_height);

    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model = glm::mat4{1.0f};
            // float rotateAngle = glm::radians(20.f) + glfwGetTime();
            // model = glm::rotate(model, rotateAngle, glm::vec3(1.0f, 0.3f, 0.5f));
    glm::mat4 view = glm::mat4{1.0f};
    view = glm::lookAt(this->Pos, this->Pos + this->Front, this->Up);
    glm::mat4 perspective = glm::mat4{1.0f};
    perspective = glm::perspective(glm::radians(this->fov), static_cast<float>(this->g_width) / static_cast<float>(this->g_height), 0.1f, 100.0f);

    glUseProgram(this->shader_id);
    glUniformMatrix4fv(glGetUniformLocation(this->shader_id, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(this->shader_id, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(this->shader_id, "perspective"), 1, GL_FALSE, glm::value_ptr(perspective));

    glBindVertexArray(this->VAO_id);
    glDrawArrays(GL_TRIANGLES,0,36);

}
