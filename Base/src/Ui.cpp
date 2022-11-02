/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ui.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:31:56 by sejpark           #+#    #+#             */
/*   Updated: 2022/11/02 16:33:13 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base/Ui.h"
#include <string>

Ui::Ui(Window &window) : window_(window) {
    IMGUI_CHECKVERSION();
    uiContext_ = ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window_.getWindow(), uiContext_);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();

    modelStatus_.modelName = "NONE";
    modelStatus_.totalVerticesCount = 0;
    modelStatus_.totalIndicesCount = 0;
    modelStatus_.totalFacesCount = 0;
    modelStatus_.fps = 0;
}

Ui::~Ui() {
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(uiContext_);
    ImPlot::DestroyContext();
}

void Ui::drawUi(Uniforms &uniforms, Objects &objects) {
    bool openSphereModal = false;
    bool openHelpModal = false;
    static bool viewFpsGraph = false;
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.16f, 0.24f, 0.43f, 1.0f)); // Menu bar background color
    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 16.0f)); // Menu bar padding
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Create")) {
            if (ImGui::MenuItem("Create Sphere")) {
                openSphereModal = true;
            }
            if (ImGui::MenuItem("Import .obj File", "Ctrl+o")) {
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", "SCOP/objects/");
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Help")) {
                openHelpModal = true;
            }
            ImGui::EndMenu();
        }
        // file dialog display
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
//                std::cout << filePathName << std::endl;
//                std::cout << filePath << std::endl;
                // action
                for (size_t i = 0; i < objects.spheres.size(); i++) {
                    delete objects.spheres[i];
                    objects.spheres.pop_back();
                }
                for (size_t i = 0; i < objects.models.size(); i++) {
                    delete objects.models[i];
                    objects.models.pop_back();
                }
                objects.models.push_back(new Model(filePathName));

                // 모델 정보 설정
                modelStatus_.modelName = ImGuiFileDialog::Instance()->GetCurrentFileName();
                modelStatus_.totalVerticesCount = 0;
                modelStatus_.totalIndicesCount = 0;
                modelStatus_.totalFacesCount = 0;
                for (size_t i = 0; i < objects.models.size(); i++)
                    setModelStatus(objects.models[i]);
            }

            // close
            ImGuiFileDialog::Instance()->Close();
        }
        ImGui::EndMainMenuBar();
    }
    //ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);

    if (openSphereModal) {
        ImGui::OpenPopup("Sphere");
    }
    if (ImGui::BeginPopupModal("Sphere")) {
        static float radius = 1.0f;
        static int sectorCount = 36;
        static int stackCount = 18;
        ImGui::SliderFloat("Radius", &radius, 0.1f, 5.0f);
        ImGui::SliderInt("Sector Count", &sectorCount, 3, 50);
        ImGui::SliderInt("Stack Count", &stackCount, 2, 50);
        if (ImGui::Button("Create")) {
            for (size_t i = 0; i < objects.spheres.size(); i++) {
                delete objects.spheres[i];
                objects.spheres.pop_back();
            }
            for (size_t i = 0; i < objects.models.size(); i++) {
                delete objects.models[i];
                objects.models.pop_back();
            }
            objects.spheres.push_back(new Sphere(radius, sectorCount, stackCount));

            // 모델 정보 설정
            modelStatus_.modelName = "Sphere";
            modelStatus_.totalVerticesCount = 0;
            modelStatus_.totalIndicesCount = 0;
            modelStatus_.totalFacesCount = 0;
            for (size_t i = 0; i < objects.spheres.size(); i++)
                setSphereStatus(objects.spheres[i]);

            ImGui::CloseCurrentPopup();
            openSphereModal = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            openSphereModal = false;
        }
        ImGui::EndPopup();
    }

    if (openHelpModal) {
        ImGui::OpenPopup("Help");
    }
    if (ImGui::BeginPopupModal("Help")) {
        if (ImGui::BeginTable("helpTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("w");
            ImGui::TableNextColumn();
            ImGui::Text("Move Forward");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("s");
            ImGui::TableNextColumn();
            ImGui::Text("Move Backward");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("a");
            ImGui::TableNextColumn();
            ImGui::Text("Move Left");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("d");
            ImGui::TableNextColumn();
            ImGui::Text("Move Right");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Left Mouse Button Drag");
            ImGui::TableNextColumn();
            ImGui::Text("Change Camera Direction");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Right Mouse Button Click + w/s");
            ImGui::TableNextColumn();
            ImGui::Text("Move Up/Down");
            ImGui::EndTable();
        }
        if (ImGui::Button("Ok")) {
            ImGui::CloseCurrentPopup();
            openHelpModal = false;
        }
        ImGui::EndPopup();
    }

    if (ImGui::Begin("Status")) {
        if (ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginTable("infoTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Model Name");
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(modelStatus_.modelName.c_str());

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Vertices Count");
                ImGui::TableNextColumn();
                ImGui::Text("%d", modelStatus_.totalVerticesCount);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Indices Count");
                ImGui::TableNextColumn();
                ImGui::Text("%d", modelStatus_.totalIndicesCount);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Faces Count");
                ImGui::TableNextColumn();
                ImGui::Text("%d", modelStatus_.totalFacesCount);
                ImGui::EndTable();
            }
        }
        if (ImGui::CollapsingHeader("Frame Time", ImGuiTreeNodeFlags_DefaultOpen)
                && modelStatus_.modelName != "NONE") {
            if (ImGui::BeginTable("fpsTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Frame Time");
                ImGui::TableNextColumn();
                ImGui::Text("%.2f ms/frame", window_.getMillisecPerFrame());
                ImGui::EndTable();
            }
            ImGui::Checkbox("view graph", &viewFpsGraph);
            static float t = 0;
            static ScrollingBuffer scrollBuf;
            static float history = 10.0f;
            t += ImGui::GetIO().DeltaTime;
            if (viewFpsGraph) {
                scrollBuf.AddPoint(t, window_.getMillisecPerFrame());
                if (ImPlot::BeginPlot("##ms/frame", ImVec2(-1, 150))) {
                    ImPlot::SetupAxes("sec", "ms/frame", ImPlotAxisFlags_RangeFit, ImPlotAxisFlags_RangeFit);
                    ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 5);
                    ImPlot::PlotLine("##NoTitle", &scrollBuf.data[0].x, &scrollBuf.data[0].y, scrollBuf.data.size(), ImPlotLineFlags_Shaded, scrollBuf.offset, 2 * sizeof(float));
                    ImPlot::EndPlot();
                }
            }
        }
    }
    ImGui::End();

    if (ImGui::Begin("Tool")) {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            Camera &cam = window_.get_camera();
            std::string ele_labels[3] = {"X: ", "Y: ", "Z: "};
            glm::vec3 camPos = cam.getPosition();
            DragFloat3("Camera Position", ele_labels, glm::value_ptr(camPos), 0.01f);
            cam.setPosition(camPos);
            ImGui::SameLine();
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
           ImGui::SliderFloat("Shininess", &uniforms.shininess, 4.0f, 256.0f);
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

struct ModelStatus Ui::setModelStatus(Model *model) {
    const std::vector<Mesh> &meshes = model->getMeshes(); 

    for (size_t i = 0; i < meshes.size(); i++) {
        modelStatus_.totalVerticesCount += meshes[i].vertices_.size();
        modelStatus_.totalIndicesCount += meshes[i].indices_.size();
    }
    modelStatus_.totalFacesCount += modelStatus_.totalIndicesCount / 3;
    return modelStatus_;
}

struct ModelStatus Ui::setSphereStatus(Sphere *sphere) {
    modelStatus_.totalVerticesCount = sphere->getInterleavedVertexCount();
    modelStatus_.totalIndicesCount = sphere->getIndexCount();
    modelStatus_.totalFacesCount = modelStatus_.totalIndicesCount / 3;
    return modelStatus_;
}
