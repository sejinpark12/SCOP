//
// Created by Sejin Park on 2022/01/26.
//

#include "Base/Camera.h"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &front, const glm::vec3 &up)
    : position_(position),
      front_(front),
      up_(up),
      worldup_(up_),
      yaw_(YAW),
      pitch_(PITCH) {
    updateCameraVectors();
}

Camera::~Camera() {}

const glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::setPosition(glm::vec3 position) {
    position_ = position;
}

void Camera::setYaw(float yaw) {
    yaw_ = yaw;
}

void Camera::setPitch(float pitch) {
    pitch_ = pitch;
}

const glm::vec3 Camera::getPosition() const {
    return position_;
}

const glm::vec3 Camera::getFront() const {
    return front_;
}

const glm::vec3 Camera::getRight() const {
    return right_;
}

const GLfloat Camera::getYaw() const {
    return yaw_;
}

const GLfloat Camera::getPitch() const {
    return pitch_;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_, worldup_));
    up_ = glm::normalize(glm::cross(right_, front_));
}
