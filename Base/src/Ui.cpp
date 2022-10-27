/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ui.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:31:56 by sejpark           #+#    #+#             */
/*   Updated: 2022/10/27 18:25:50 by sejpark          ###   ########.fr       */
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

void Ui::drawUi(Camera &cam, Uniforms &uniforms, std::vector<Model*> &models) {
    ImGui::Begin("Objects", NULL, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Create")) {
            if (ImGui::MenuItem("Sphere", "Ctrl+s"))  { }
            if (ImGui::MenuItem("Open .obj", "Ctrl+o")) {
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", "SCOP/objects/");
            }

            ImGui::EndMenu();
        }
        // display
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
//                std::cout << filePathName << std::endl;
//                std::cout << filePath << std::endl;
                // action
                for (int i = 0; i < models.size(); i++) {
                    delete models[i];
                    models.pop_back();
                }
                models.push_back(new Model(filePathName));
            }

            // close
            ImGuiFileDialog::Instance()->Close();
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
            std::string ele_labels[3] = {"X: ", "Y: ", "Z: "};
            glm::vec3 camPos = cam.getPosition();
            DragFloat3("Camera Position", ele_labels, glm::value_ptr(camPos), 0.01f);
            cam.setPosition(camPos);
            if (ImGui::Button("Reset")) {
	            cam.setPosition(glm::vec3(0.0f, 0.0f, 5.0f)) ;
                cam.setYaw(-90.0f);
                cam.setPitch(0.0f);
            }
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Background Color", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit4("Background", glm::value_ptr(uniforms.backGroundColor));
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::RadioButton("Directional(Gouraud)", &uniforms.selectLight, DIRECT_GOURAUD);
            ImGui::RadioButton("Directional(Phong)", &uniforms.selectLight, DIRECT_PHONG);
            ImGui::RadioButton("Point", &uniforms.selectLight, POINT);
            ImGui::SameLine();
            ImGui::RadioButton("Spotlight", &uniforms.selectLight, SPOTLIGHT);
            ImGui::Separator();

            if (uniforms.selectLight == DIRECT_GOURAUD || uniforms.selectLight == DIRECT_PHONG) {
                if (ImGui::CollapsingHeader(uniforms.selectLight == DIRECT_GOURAUD ? "Directional Light(Gouraud)" : "Directional Light(Phong)", ImGuiTreeNodeFlags_DefaultOpen)) {
                    std::string ele_labels[3] = {"X: ", "Y: ", "Z: "};
                    DragFloat3("Light Direction", ele_labels, glm::value_ptr(uniforms.lightDir), 0.01f);
                }
            }
            else {
                if (ImGui::CollapsingHeader(uniforms.selectLight == POINT ? "Point Light" : "Spotlight", ImGuiTreeNodeFlags_DefaultOpen)) {
                    std::string ele_labels[3] = {"X: ", "Y: ", "Z: "};
                    DragFloat3("Light Position", ele_labels, glm::value_ptr(uniforms.lightPos), 0.1f);
                    if (uniforms.selectLight == SPOTLIGHT) {
                        DragFloat3("Light Direction", ele_labels, glm::value_ptr(uniforms.spotDir), 0.01f);
                        ImGui::SliderFloat("Cutoff Angle", &uniforms.spotCutOffAngle, 0.0f, 90.0f);
                    }
                }
            }
       }
       ImGui::Separator();
       if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
           //ImGui::Text("Diffuse Color");
           ImGui::ColorEdit4("Diffuse", glm::value_ptr(uniforms.diffuse));
           //ImGui::Text("Ambient");
           ImGui::SliderFloat("Ambient", &uniforms.ambient, 0.0f, 1.0f);
           //ImGui::Text("Shininess");
           ImGui::SliderFloat("Shininess", &uniforms.shininess, 4.0f, 128.0f);
           //ImGui::Text("Specular Color");
           ImGui::ColorEdit4("Specular", glm::value_ptr(uniforms.specular));
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

void Ui::DragFloat3(const std::string label,
                    const std::string ele_labels[3],
                    float v[3],
                    float v_speed,
                    float v_min,
                    float v_max, 
                    const std::string format,
                    ImGuiSliderFlags flags) {
    ImGui::TextUnformatted(label.c_str());
    ImGui::BeginGroup();
    ImGui::PushID(label.c_str());
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::Indent(5.0f);
    ImGui::PushID("0");
    ImGui::DragFloat("", &v[0], v_speed, v_min, v_max,
                    (ele_labels[0] + format).c_str(), flags);
    ImGui::SameLine();
    ImGui::PopID();
    ImGui::PopItemWidth();
    ImGui::PushID("1");
    ImGui::SameLine();
    ImGui::DragFloat("", &v[1], v_speed, v_min, v_max,
                    (ele_labels[1] + format).c_str(), flags);
    ImGui::SameLine();
    ImGui::PopID();
    ImGui::PopItemWidth();
    ImGui::PushID("2");
    ImGui::SameLine();
    ImGui::DragFloat("", &v[2], v_speed, v_min, v_max,
                    (ele_labels[2] + format).c_str(), flags);
    ImGui::SameLine();
    ImGui::PopID();
    ImGui::PopItemWidth();
    ImGui::PopID();
    ImGui::EndGroup();
}
