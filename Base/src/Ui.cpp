/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ui.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:31:56 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/25 15:38:57 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base/Ui.h"

Ui::Ui(const Window &window) {
    IMGUI_CHECKVERSION();
    uiContext_ = ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), uiContext_);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();
}

Ui::~Ui() {
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(uiContext_);
}

void Ui::drawUi(Uniforms &uniforms) {
    ImGui::Begin("Objects", NULL, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Select")) {
            if (ImGui::MenuItem("Sphere", "Ctrl+s"))  { }
            if (ImGui::MenuItem("Open .obj", "Ctrl+o")) { /* Do stuff */ }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
        ImGui::Separator();
        if (ImGui::CollapsingHeader("FPS", ImGuiTreeNodeFlags_DefaultOpen)) {
        }
    }
    ImGui::End();

    if (ImGui::Begin("Help")) {
        ImGui::Text("This is first text...");
    }
    ImGui::End();

    if (ImGui::Begin("Tool")) {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Background Color", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit4("Background Color", glm::value_ptr(uniforms.backGroundColor));
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::RadioButton("Directional(Gouraud)", &uniforms.selectLight, DIRECT_GOURAUD); ImGui::SameLine();
            ImGui::RadioButton("Directional(Phong)", &uniforms.selectLight, DIRECT_PHONG);
            ImGui::RadioButton("Point", &uniforms.selectLight, POINT); ImGui::SameLine();
            ImGui::RadioButton("Spotlight", &uniforms.selectLight, SPOTLIGHT);
            ImGui::Separator();
            if (uniforms.selectLight == DIRECT_GOURAUD) {
                ImGui::Text("Light Direction");
                ImGui::BeginGroup();
                ImGui::PushID("vec");
                ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
                ImGui::Indent(5.0f);
                ImGui::PushID("0");
                ImGui::DragFloat("", &uniforms.lightDir.x, 0.01f, 0.0f, 0.0f, "X: %.3f");
                ImGui::SameLine();
                ImGui::PopID();
                ImGui::PopItemWidth();
                ImGui::PushID("1");
                ImGui::SameLine();
                ImGui::DragFloat("", &uniforms.lightDir.y, 0.01f, 0.0f, 0.0f, "Y: %.3f");
                ImGui::SameLine();
                ImGui::PopID();
                ImGui::PopItemWidth();
                ImGui::PushID("2");
                ImGui::SameLine();
                ImGui::DragFloat("", &uniforms.lightDir.z, 0.01f, 0.0f, 0.0f, "Z: %.3f");
                ImGui::SameLine();
                ImGui::PopID();
                ImGui::PopItemWidth();
                ImGui::PopID();
                ImGui::EndGroup();

                //ImGui::DragFloat3("Direction", glm::value_ptr(lightDir), 0.01f);
           }
           if (uniforms.selectLight == DIRECT_PHONG) {
                ImGui::Text("Directional Light(Phong)");
                ImGui::DragFloat3("Direction", glm::value_ptr(uniforms.lightDir), 0.01f);
           }
           else if (uniforms.selectLight == POINT) {
                ImGui::Text("Point Light");
                ImGui::DragFloat3("Position", glm::value_ptr(uniforms.lightPos), 0.1f);
           }
           else if (uniforms.selectLight == SPOTLIGHT) {
                ImGui::Text("Spotlight");
                ImGui::DragFloat3("Position", glm::value_ptr(uniforms.lightPos), 0.1f);
                ImGui::DragFloat3("Direction", glm::value_ptr(uniforms.spotDir), 0.01f);
                ImGui::SliderFloat("Cutoff Angle", &uniforms.spotCutOffAngle, 0.0f, 90.0f);
           }
       }
       ImGui::Separator();
       if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
           //ImGui::Text("Diffuse Color");
           ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(uniforms.diffuse));
           //ImGui::Text("Ambient");
           ImGui::SliderFloat("Ambient", &uniforms.ambient, 0.0f, 1.0f);
           //ImGui::Text("Shininess");
           ImGui::SliderFloat("Shininess", &uniforms.shininess, 4.0f, 128.0f);
           //ImGui::Text("Specular Color");
           ImGui::ColorEdit4("Specular Color", glm::value_ptr(uniforms.specular));
       }
       ImGui::Separator();
    }
    ImGui::End();
}

void Ui::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
};

void Ui::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Ui::isCaptureMouse() {
    return ImGui::GetIO().WantCaptureMouse;
}

void Ui::processEvent(SDL_Event *event) {
    ImGui_ImplSDL2_ProcessEvent(event);
}
