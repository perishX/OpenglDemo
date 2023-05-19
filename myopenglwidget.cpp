#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent):QOpenGLWidget{parent}{
    this->grabKeyboard();
    this->viewer.updateDirection();

}

void MyOpenGLWidget::resizeEvent(QResizeEvent *event){
    this->g_width=this->width();
    this->g_height=this->height();
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event){
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
    initializeOpenGLFunctions();

    this->cube.init();
    this->shader.init("C:/Users/73965/Documents/demo/shaders/shader.vert","C:/Users/73965/Documents/demo/shaders/shader.frag");
    this->ID=this->shader.getID();

    glEnable(GL_DEPTH_TEST);
}

void MyOpenGLWidget::resizeGL(int w, int h){
}

void MyOpenGLWidget::paintGL(){
    glViewport(0,0,this->g_width,this->g_height);

    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model = glm::mat4{1.0f};
    glm::mat4 view = glm::mat4{1.0f};
    view = this->viewer.getViewMatrix();
    glm::mat4 perspective = glm::mat4{1.0f};
    perspective = glm::perspective(glm::radians(this->viewer.getFov()), static_cast<float>(this->g_width) / static_cast<float>(this->g_height), 0.1f, 100.0f);

    glUseProgram(this->ID);
    this->shader.setMatrix4f("model", 1, glm::value_ptr(model));
    this->shader.setMatrix4f("view", 1, glm::value_ptr(view));
    this->shader.setMatrix4f("perspective", 1, glm::value_ptr(perspective));

    this->cube.Draw();

}

