//
// Created by Sejin Park on 2022/02/03.
//

#ifndef INPUT_H
#define INPUT_H

#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include "Camera.h"

// 기본 입력 설정값
const float KEYBOARD_SPEED = 0.01;
const float MOUSE_SPEED = 0.1;

class Input {
private:
    float keyboard_speed_;
    float mouse_speed_;
    float last_xpos_;
    float last_ypos_;
    float xoffset_;
    float yoffset_;

public:
    /// 생성자.
    /// \param last_xpos 이전 프레임의 마우스 x좌표입니다.
    /// \param last_ypos 이전 프레임의 마우스 y좌표입니다.
    /// \param keyboard_speed 키보드 이동속도입니다.
    /// \param mouse_speed 마우스 이동속도입니다.
    explicit Input(float last_xpos, float last_ypos, float keyboard_speed = KEYBOARD_SPEED, float mouse_speed = MOUSE_SPEED);

    /// 소멸자.
    ~Input();

    /// 키보드 입력을 처리합니다.
    /// \param state 키보드의 입력 상태입니다.
    /// \param delta_time 이전 프레임을 완료하는데 걸린 시간입니다.
    /// \param cam 키보드 입력이 반영되는 카메라 객체입니다.
    void processKeyboard(const Uint8 *state, const float &delta_time, Camera &cam);

    /// 마우스 입력을 처리합니다.
    /// \param xpos 마우스의 현재 x 좌표입니다.
    /// \param ypos 마우스의 현재 y 좌표입니다.
    /// \param cam 마우스 입력이 반영되는 카메라 객체입니다.
    void processMouse(const int &xpos, const int &ypos, Camera &cam);
};

#endif ///INPUT_H
