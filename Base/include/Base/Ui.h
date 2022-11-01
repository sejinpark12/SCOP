/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:26:38 by sejpark           #+#    #+#             */
/*   Updated: 2022/11/01 15:26:34 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
#define UI_H

#include <SDL_syswm.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuiFileDialog.h>
#include <implot.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include "Window.h"
#include "Uniforms.h"
#include "Camera.h"
#include "Objects.h"

struct ModelStatus {
    std::string modelName;
    unsigned int totalVerticesCount;
    unsigned int totalIndicesCount;
    unsigned int totalFacesCount;
    unsigned int fps;
};

struct ScrollingBuffer {
    int maxSize;
    int offset;
    ImVector<ImVec2> data;

    ScrollingBuffer(int max_size = 8000) {
        maxSize = max_size;
        offset = 0;
        data.reserve(maxSize);
    }

    void AddPoint(float x, float y) {
        if (data.size() < maxSize)
            data.push_back(ImVec2(x, y));
        else {
            data[offset] = ImVec2(x, y);
            offset = (offset + 1) % maxSize;
        }
    }

    void Erase() {
        if (data.size() > 0) {
            data.shrink(0);
            offset = 0;
        }
    }
};

/// UI 클래스
class Ui {
public:
    /// 생성자
    explicit Ui(Window &window);

    /// 소멸자
    ~Ui();

//    void drawUi(Camera &cam, Uniforms &uniforms, std::vector<Model*> &models,
//                std::vector<Sphere*> &spheres);

    void drawUi(Uniforms &uniforms, Objects &objects);

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

    struct ModelStatus setModelStatus(Model *model);
    struct ModelStatus setSphereStatus(Sphere *sphere);

private:
    Window &window_;
    ImGuiContext *uiContext_;
    struct ModelStatus modelStatus_;
};

#endif ///UI_H
