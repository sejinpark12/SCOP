//
// Created by Sejin Park on 2021/12/29.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>

// 기본 카메라 설정값
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ZOOM = 45.0f;

/// 카메라 클래스.
class Camera {
private:
    // 카메라 속성
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 worldup_;
    // 오일러 각
    float yaw_;
    float pitch_;

public:
    /// 생성자.
    /// \param position 생성할 카메라의 위치입니다.
    /// \param front 생성할 카메라의 front 벡터입니다.
    /// \param up 생성할 카메라의 up 벡터입니다.
    explicit Camera(
        const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 &front = glm::vec3(0.0f, 0.0f, -1.0f),
        const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f));

    /// 소멸자.
    ~Camera();

    /// 오일러 각과 LookAt 행렬로 계산한 View 행렬을 반환합니다.
    /// \return View 행렬입니다.
    [[nodiscard]] const glm::mat4 getViewMatrix() const;

    void setPosition(glm::vec3 position);
    void setYaw(float yaw);
    void setPitch(float pitch);

    const glm::vec3 getPosition() const;
    const glm::vec3 getFront() const;
    const glm::vec3 getRight() const;
    const float getYaw() const;
    const float getPitch() const;

    /// 업데이트 된 오일러 각으로 front, right, up 벡터를 계산합니다. 
    void updateCameraVectors();
};

#endif ///CAMERA_H
