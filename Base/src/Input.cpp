//
// Created by Sejin Park on 2022/02/04.
//

#include "Base/Input.h"

Input::Input(float last_xpos, float last_ypos, float keyboard_speed, float mouse_speed)
    : last_xpos_(last_xpos),
      last_ypos_(last_ypos),
      keyboard_speed_(keyboard_speed),
      mouse_speed_(mouse_speed) {}

Input::~Input() {}

void Input::processKeyboard(const Uint8 *state, const float &delta_time, Camera &cam) {
    const float velocity = keyboard_speed_ * delta_time;
    if (state[SDL_SCANCODE_W])
        cam.setPosition(cam.getPosition() + cam.getFront() * velocity);
    if (state[SDL_SCANCODE_S])
        cam.setPosition(cam.getPosition() - cam.getFront() * velocity);
    if (state[SDL_SCANCODE_A])
        cam.setPosition(cam.getPosition() - cam.getRight() * velocity);
    if (state[SDL_SCANCODE_D])
        cam.setPosition(cam.getPosition() + cam.getRight() * velocity);
}

void Input::processMouse(const int &xpos, const int &ypos, Camera &cam) {
    xoffset_ = xpos - last_xpos_;
    yoffset_ = last_ypos_ - ypos;
    last_xpos_ = xpos;
    last_ypos_ = ypos;

    xoffset_ *= mouse_speed_;
    yoffset_ *= mouse_speed_;

    cam.setYaw(cam.getYaw() + xoffset_);
    cam.setPitch(cam.getPitch() + yoffset_);

    if (cam.getPitch() > 89.0f)
        cam.setPitch(89.0f);
    if (cam.getPitch() < -89.0f)
        cam.setPitch(-89.0f);
}
