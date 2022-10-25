/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:26:38 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/25 18:26:20 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
#define UI_H

#include <SDL_syswm.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "Window.h"
#include "Uniforms.h"
#include "Camera.h"

/// UI 클래스
class Ui {
public:
    /// 생성자
    explicit Ui(const Window &window);

    /// 소멸자
    ~Ui();

    void drawUi(Camera &cam, Uniforms &uniforms);
    static void newFrame();
    static void render();
    static bool isCaptureMouse();
    static void processEvent(SDL_Event *event);

private:
    void DragFloat3(const std::string label,
                    const std::string ele_labels[3],
                    float v[3],
                    float v_speed = 1.0f,
                    float v_min = 0.0f,
                    float v_max = 0.0f, 
                    const std::string format = "%.3f",
                    ImGuiSliderFlags flags = 0);

private:
    ImGuiContext *uiContext_;
};

#endif ///UI_H
