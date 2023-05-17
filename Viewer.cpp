#include "Viewer.h"

Viewer::Viewer(){

}

Viewer::~Viewer(){

}

void Viewer::updateDirection(float yawOffset, float pitchOffset){
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
