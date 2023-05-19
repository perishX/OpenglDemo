#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <QOpenGLFunctions_3_3_Core>

class Cube : public QOpenGLFunctions_3_3_Core
{
private:
    unsigned int VAO;
    unsigned int VBO;
    std::vector<float> vertices;
    void setupMesh();

public:
    Cube();
    Cube(std::vector<float> vertices);
    ~Cube();
    void init();
    void Draw(bool isLineMode = false);
};
#endif // CUBE_H
