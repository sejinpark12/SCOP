/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 00:11:22 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/20 00:11:47 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base/Input.h"
#include <iostream>

Input::Input(float last_xpos, float last_ypos, float keyboard_speed, float mouse_speed)
    : last_xpos_(last_xpos),
      last_ypos_(last_ypos),
      keyboard_speed_(keyboard_speed),
      mouse_speed_(mouse_speed),
      mouseRBtnDown_(false) {}

Input::~Input() {}

void Input::processKeyboard(const Uint8 *state, const float &delta_time, Camera &cam) {
    const float velocity = keyboard_speed_ * delta_time;
    if (state[SDL_SCANCODE_W]) {
        if (mouseRBtnDown_ == false)
            cam.setPosition(cam.getPosition() + cam.getFront() * velocity);
        else
            cam.setPosition(cam.getPosition() + cam.getUp() * velocity);
    }
    if (state[SDL_SCANCODE_S]) {
        if (mouseRBtnDown_ == false)
            cam.setPosition(cam.getPosition() - cam.getFront() * velocity);
        else
            cam.setPosition(cam.getPosition() - cam.getUp() * velocity);
    }
    if (state[SDL_SCANCODE_A])
        cam.setPosition(cam.getPosition() - cam.getRight() * velocity);
    if (state[SDL_SCANCODE_D])
        cam.setPosition(cam.getPosition() + cam.getRight() * velocity);
}

void Input::processMouse(const Uint32 &mouseBtns, const int &xpos, const int &ypos, Camera &cam) {
    if ((mouseBtns & SDL_BUTTON_LMASK) != 0)
        processCamAngle(xpos, ypos, cam);
    else if ((mouseBtns & SDL_BUTTON_RMASK) != 0)
        mouseRBtnDown_ = true;
}
void Input::processCamAngle(const int &xpos, const int &ypos, Camera &cam) {
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

void Input::setLastMousePos(const float last_xpos, const float last_ypos) {
    last_xpos_ = last_xpos;
    last_ypos_ = last_ypos;
}

void Input::setMouseRBtnDown(bool rBtn) {
    mouseRBtnDown_ = rBtn;
}
