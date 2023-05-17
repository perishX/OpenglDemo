#ifndef VIEWER_H
#define VIEWER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Viewer{
public:
    Viewer();
    ~Viewer();
    void updateDirection(float yawOffset, float pitchOffset);
private:
    glm::vec3 Pos{1,1,5};
    glm::vec3 Front{0,0,-1};
    glm::vec3 Up{0,1,0};
    glm::vec3 Right{};
    glm::vec3 WorldUp{0,1,0};
    float yaw{-90};
    float pitch{};
    float lastX{};
    float lastY{};
    float fov{45.f};
    float forwardFlag{};
    float rightFlag{};
    float upFlag{};
};
#endif // VIEWER_H
