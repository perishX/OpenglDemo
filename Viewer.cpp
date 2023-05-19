#include "Viewer.h"

Viewer::Viewer(){

}

Viewer::~Viewer(){

}

void Viewer::setForwardFlag(float flag){
    this->forwardFlag=flag;
}

void Viewer::setRightFlag(float flag){
    this->rightFlag=flag;
}

void Viewer::setUpFlag(float flag){
    this->upFlag=flag;
}

float Viewer::getForwardFlag(){
    return this->forwardFlag;
}

float Viewer::getRightFlag(){
    return this->rightFlag;
}

float Viewer::getUpFlag(){
    return this->upFlag;
}

float Viewer::getFov(){
    return this->fov;
}

glm::mat4 Viewer::getViewMatrix(){
    return glm::lookAt(this->Pos, this->Pos + this->Front, this->Up);
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

void Viewer::move(){
    this->Pos += (this->Front * this->forwardFlag + this->Right * this->rightFlag + this->WorldUp * this->upFlag) * 0.1f;
}

void Viewer::zoom(float offset){
    if (this->fov >= 1.f && this->fov <= 45.f){
        this->fov += offset;
    }
    if (this->fov < 1.f){
        this->fov = 1.f;
    }
    if (this->fov > 45.f){
        this->fov = 45.f;
    }
}
