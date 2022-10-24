//
// Created by Daemyung Jang on 2021/10/19.
// Updated by Sejin Park on 2022/10/19.
//

#include <__nullptr>
#include <array>
#include <vector>
#include <spdlog/spdlog.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
#include <Base/Window.h>
#include <Base/utility.h>
#include <Base/Shader.h>
#include <Base/Model.h>
#include <Base/GeometricShapes/Sphere.h>

struct App {
    EGLDisplay display{EGL_NO_DISPLAY};
    EGLConfig config{nullptr};
    EGLContext context{EGL_NO_CONTEXT};
    EGLSurface surface{EGL_NO_SURFACE};
    std::vector<GLuint> programs;
    GLint model_location{0};
    GLint view_location{0};
    GLint projection_location{0};
    Model *model{nullptr};
    Sphere *sphere{nullptr};
    ImGuiContext *imguiContext{nullptr};
};

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const float fov = 45.0f;

glm::vec4 backGroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
glm::vec4 diffuse = glm::vec4(0.47f, 0.30f, 0.64f, 0.0f);
float ambient = 0.25f;
float shininess = 80.0f;
glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
glm::vec4 lightDir = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
glm::vec4 lightPos = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 spotDir = glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);
float spotCutOffAngle = 30.0f;
static int e = 1;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    App app{};
    Window window{{.title = "SCOP", .size = {SCR_WIDTH, SCR_HEIGHT}}};

    window.run([&app, &window] {
            startup(app,window);
            printAPIInfo(app);


            GL_TEST(glEnable(GL_DEPTH_TEST));
//            Shader shader({home() / "SCOP/res/triangle.vert",
//                           home() / "SCOP/res/unlit.frag"});
            Shader dirGouraudshader({home() / "SCOP/res/directionlight/gouraud.vert",
                           home() / "SCOP/res/directionlight/gouraud.frag"});
            Shader dirPhongShader({home() / "SCOP/res/directionlight/phong.vert",
                           home() / "SCOP/res/directionlight/phong.frag"});
            Shader pointShader({home() / "SCOP/res/pointlight/pointlight.vert",
                           home() / "SCOP/res/pointlight/pointlight.frag"});
            Shader spotShader({home() / "SCOP/res/spotlight/spotlight.vert",
                           home() / "SCOP/res/spotlight/spotlight.frag"});
            app.programs.push_back(dirGouraudshader.getProgramId());
            app.programs.push_back(dirPhongShader.getProgramId());
            app.programs.push_back(pointShader.getProgramId());
            app.programs.push_back(spotShader.getProgramId());
            //app.model = new Model(home() / "SCOP/res/objects/teapot.obj");
            app.sphere = new Sphere();
            //app.sphere = new Sphere(1, 36, 18, false);
       },
       [] {},
       [&app, &window] {
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
                    ImGui::ColorEdit4("Background Color", glm::value_ptr(backGroundColor));
               }
               ImGui::Separator();
               if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
                   ImGui::RadioButton("Directional(Gouraud)", &e, 0); ImGui::SameLine();
                   ImGui::RadioButton("Directional(Phong)", &e, 1);
                   ImGui::RadioButton("Point", &e, 2); ImGui::SameLine();
                   ImGui::RadioButton("Spotlight", &e, 3);
                   ImGui::Separator();
                   if (e == 0) {
                       ImGui::Separator();
                       ImGui::Text("Light Direction");
                       ImGui::BeginGroup();
                       ImGui::PushID("vec");
                       ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
                       ImGui::Indent(5.0f);
                       ImGui::PushID("0");
                       ImGui::DragFloat("", &lightDir.x, 0.01f, 0.0f, 0.0f, "X: %.3f");
                       ImGui::SameLine();
                       ImGui::PopID();
                       ImGui::PopItemWidth();
                       ImGui::PushID("1");
                       ImGui::SameLine();
                       ImGui::DragFloat("", &lightDir.y, 0.01f, 0.0f, 0.0f, "Y: %.3f");
                       ImGui::SameLine();
                       ImGui::PopID();
                       ImGui::PopItemWidth();
                       ImGui::PushID("2");
                       ImGui::SameLine();
                       ImGui::DragFloat("", &lightDir.z, 0.01f, 0.0f, 0.0f, "Z: %.3f");
                       ImGui::SameLine();
                       ImGui::PopID();
                       ImGui::PopItemWidth();
                       ImGui::PopID();
                       ImGui::EndGroup();

                       //ImGui::DragFloat3("Direction", glm::value_ptr(lightDir), 0.01f);
                   }
                   if (e == 1) {
                       ImGui::Text("Directional Light(Phong)");
                       ImGui::DragFloat3("Direction", glm::value_ptr(lightDir), 0.01f);
                   }
                   else if (e == 2) {
                       ImGui::Text("Point Light");
                       ImGui::DragFloat3("Position", glm::value_ptr(lightPos), 0.1f);
                   }
                   else if (e == 3) {
                       ImGui::Text("Spotlight");
                       ImGui::DragFloat3("Position", glm::value_ptr(lightPos), 0.1f);
                       ImGui::DragFloat3("Direction", glm::value_ptr(spotDir), 0.01f);
                       ImGui::SliderFloat("Cutoff Angle", &spotCutOffAngle, 0.0f, 90.0f);
                   }
               }
               ImGui::Separator();
               if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
                   //ImGui::Text("Diffuse Color");
                   ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(diffuse));
                   //ImGui::Text("Ambient");
                   ImGui::SliderFloat("Ambient", &ambient, 0.0f, 1.0f);
                   //ImGui::Text("Shininess");
                   ImGui::SliderFloat("Shininess", &shininess, 0.0f, 128.0f);
                   //ImGui::Text("Specular Color");
                   ImGui::ColorEdit4("Specular Color", glm::value_ptr(specular));
               }
               ImGui::Separator();
           }
           ImGui::End();

           GL_TEST(glClearColor(backGroundColor.r, backGroundColor.g, backGroundColor.b, backGroundColor.a));
           GL_TEST(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

           GL_TEST(glViewport(0, 0, window.size().x * window.dpi(), 
                       window.size().y * window.dpi()));

           GL_TEST(glUseProgram(app.programs[e]));
           
           glm::mat4 projection = glm::perspective(glm::radians(fov),
                   static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
                   0.001f, 100.0f) ;
           app.projection_location = glGetUniformLocation(app.programs[e], "projection");
           GL_TEST(glUniformMatrix4fv(app.projection_location, 1, GL_FALSE,
                       &projection[0][0]));
           glm::mat4 view = window.get_camera().getViewMatrix();
           app.view_location = glGetUniformLocation(app.programs[e], "view");
           GL_TEST(glUniformMatrix4fv(app.view_location, 1, GL_FALSE, &view[0][0]));
           glm::mat4 model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
           app.model_location = glGetUniformLocation(app.programs[e], "model");
           GL_TEST(glUniformMatrix4fv(app.model_location, 1, GL_FALSE, &model[0][0]));

           GLint lightDir_location{0};
           glm::vec4 lightDirection = glm::normalize(lightDir);
           lightDir_location = glGetUniformLocation(app.programs[e], "l_dir");
           GL_TEST(glUniform4fv(lightDir_location, 1, glm::value_ptr(lightDirection)));

           GLint lightPos_location{0};
           lightPos_location = glGetUniformLocation(app.programs[e], "l_pos");
           GL_TEST(glUniform4fv(lightPos_location, 1, glm::value_ptr(lightPos)));

           GLint camPos_location{0};
           glm::vec4 camPosition = glm::vec4(window.get_camera().getPosition(), 1.0);
           camPos_location = glGetUniformLocation(app.programs[e], "campos");
           GL_TEST(glUniform4fv(camPos_location, 1, glm::value_ptr(camPosition)));

           GLint spotDir_location{0};
           spotDir_location = glGetUniformLocation(app.programs[e], "l_spotDir");
           GL_TEST(glUniform4fv(spotDir_location, 1, glm::value_ptr(spotDir)));
//           SPDLOG_INFO("spotDirection: {}, {}, {}, {}", spotDirection.x, spotDirection.y, spotDirection.z, spotDirection.w);

           GLint spotCutOff_location{0};
           float spotCutOff = glm::cos(glm::radians(spotCutOffAngle));
           spotCutOff_location = glGetUniformLocation(app.programs[e], "l_spotCutOff");
           GL_TEST(glUniform1fv(spotCutOff_location, 1, &spotCutOff));

           GLint diffuse_location{0};
           diffuse_location = glGetUniformLocation(app.programs[e], "diffuse");
           GL_TEST(glUniform4fv(diffuse_location, 1, glm::value_ptr(diffuse)));

           GLint ambient_location{0};
           ambient_location = glGetUniformLocation(app.programs[e], "ambient");
           GL_TEST(glUniform1fv(ambient_location, 1, &ambient));

           GLint shininess_location{0};
           shininess_location = glGetUniformLocation(app.programs[e], "shininess");
           GL_TEST(glUniform1fv(shininess_location, 1, &shininess));

           GLint specular_location{0};
           specular_location = glGetUniformLocation(app.programs[e], "specular");
           GL_TEST(glUniform4fv(specular_location, 1, glm::value_ptr(specular)));
            
           app.sphere->draw(app.programs[e]);

           GL_TEST(glBindVertexArray(0));
           GL_TEST(glUseProgram(0));

           ImGui::Render();
           ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

           EGL_TEST(eglSwapBuffers(app.display, app.surface));
       },
       [&app] {
           for (int i = 0; i < app.programs.size(); i++)
               GL_TEST(glDeleteProgram(app.programs[i]));
           std::vector<GLuint>().swap(app.programs);

           //app.model->clearModel();
           //delete app.model;
           delete app.sphere;
           shutdown(app);
       });

    return 0;
}
