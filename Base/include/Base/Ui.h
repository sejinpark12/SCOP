/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:26:38 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/25 15:19:33 by sejpark          ###   ########.fr       */
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
#include "Window.h"
#include "Uniforms.h"

extern struct Uniforms uniforms;

/// UI 클래스
class Ui {
public:
    /// 생성자
    explicit Ui(const Window &window);

    /// 소멸자
    ~Ui();

    void drawUi(Uniforms &uniforms);
    static void newFrame();
    static void render();
    static bool isCaptureMouse();
    static void processEvent(SDL_Event *event);



private:
    ImGuiContext *uiContext_;
};

#endif ///UI_H
