#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>
#include <QMouseEvent>
#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Viewer.h"
#include "Shader.h"
#include "Cube.h"

class MyOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent=nullptr);
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w,int h);
    virtual void paintGL();
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

private:
    float lastX{};
    float lastY{};
    int g_width{1920};
    int g_height{964};

    Viewer viewer{};
    Shader shader{};
    unsigned int ID{};
    Cube cube{};
signals:
};

#endif // MYOPENGLWIDGET_H
