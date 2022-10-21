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
    GLuint program{0};
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

glm::vec4 backGroundColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
glm::vec4 diffuse = glm::vec4(0.3f, 0.2f, 0.4f, 0.0f);
float ambient = 0.25f;
float shininess = 60.0f;
glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
glm::vec4 lightDir = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
glm::vec4 lightPos = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    App app{};
    Window window{{.title = "SCOP", .size = {SCR_WIDTH, SCR_HEIGHT}}};

    window.run([&app, &window] {
            startup(app,window);
            printAPIInfo(app);


            GL_TEST(glEnable(GL_DEPTH_TEST));
//            Shader shader({home() / "SCOP/res/triangle.vert",
//                           home() / "SCOP/res/unlit.frag"});
//            Shader shader({home() / "SCOP/res/gouraud.vert",
//                           home() / "SCOP/res/gouraud.frag"});
//            Shader shader({home() / "SCOP/res/phong.vert",
//                           home() / "SCOP/res/phong.frag"});
            Shader shader({home() / "SCOP/res/pointlight.vert",
                           home() / "SCOP/res/pointlight.frag"});
            app.program = shader.getProgramId();
            //app.model = new Model(home() / "SCOP/res/objects/teapot.obj");
            app.sphere = new Sphere();
            //app.sphere = new Sphere(1, 36, 18, false);
       },
       [] {},
       [&app, &window] {
           if (ImGui::Begin("my first ImGui window")) {
                ImGui::Text("This is first text...");
           }
           ImGui::End();
           if (ImGui::Begin("ui window")) {
                ImGui::ColorEdit4("clear color", glm::value_ptr(backGroundColor));
           }
           ImGui::End();
           if (ImGui::Begin("light window")) {
                ImGui::ColorEdit4("light direction", glm::value_ptr(lightDir));
                ImGui::ColorEdit4("light position", glm::value_ptr(lightPos));
           }
           ImGui::End();
           if (ImGui::Begin("sphere window")) {
                ImGui::ColorEdit4("diffuse color", glm::value_ptr(diffuse));
                ImGui::SliderFloat("ambient", &ambient, 0.0f, 1.0f);
                ImGui::SliderFloat("shininess", &shininess, 0.0f, 128.0f);
                ImGui::ColorEdit4("specular color", glm::value_ptr(specular));
           }
           ImGui::End();

           GL_TEST(glClearColor(backGroundColor.r, backGroundColor.g, backGroundColor.b, backGroundColor.a));
           GL_TEST(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

           GL_TEST(glViewport(0, 0, window.size().x * window.dpi(), 
                       window.size().y * window.dpi()));

           GL_TEST(glUseProgram(app.program));
           
           glm::mat4 projection = glm::perspective(glm::radians(fov),
                   static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
                   0.001f, 100.0f) ;
           app.projection_location = glGetUniformLocation(app.program, "projection");
           GL_TEST(glUniformMatrix4fv(app.projection_location, 1, GL_FALSE,
                       &projection[0][0]));
           glm::mat4 view = window.get_camera().getViewMatrix();
           app.view_location = glGetUniformLocation(app.program, "view");
           GL_TEST(glUniformMatrix4fv(app.view_location, 1, GL_FALSE, &view[0][0]));
           glm::mat4 model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
           app.model_location = glGetUniformLocation(app.program, "model");
           GL_TEST(glUniformMatrix4fv(app.model_location, 1, GL_FALSE, &model[0][0]));

//           GLint lightDir_location{0};
//           glm::vec4 lightDirection = glm::normalize(view * lightDir);
//           lightDir_location = glGetUniformLocation(app.program, "l_dir");
//           GL_TEST(glUniform4fv(lightDir_location, 1, glm::value_ptr(lightDirection)));

           GLint lightPos_location{0};
           glm::vec4 lightPosition = glm::normalize(view * lightPos);
           lightPos_location = glGetUniformLocation(app.program, "l_pos");
           GL_TEST(glUniform4fv(lightPos_location, 1, glm::value_ptr(lightPosition)));

           GLint diffuse_location{0};
           diffuse_location = glGetUniformLocation(app.program, "diffuse");
           GL_TEST(glUniform4fv(diffuse_location, 1, glm::value_ptr(diffuse)));

           GLint ambient_location{0};
           ambient_location = glGetUniformLocation(app.program, "ambient");
           GL_TEST(glUniform1fv(ambient_location, 1, &ambient));

           GLint shininess_location{0};
           shininess_location = glGetUniformLocation(app.program, "shininess");
           GL_TEST(glUniform1fv(shininess_location, 1, &shininess));

           GLint specular_location{0};
           specular_location = glGetUniformLocation(app.program, "specular");
           GL_TEST(glUniform4fv(specular_location, 1, glm::value_ptr(specular)));
            
           //app.model->Draw(app.program);
           app.sphere->draw(app.program);

           GL_TEST(glBindVertexArray(0));
           GL_TEST(glUseProgram(0));

           ImGui::Render();
           ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

           EGL_TEST(eglSwapBuffers(app.display, app.surface));
       },
       [&app] {
           GL_TEST(glDeleteProgram(app.program));
           app.program = 0;

           //app.model->clearModel();
           //delete app.model;
           delete app.sphere;
           shutdown(app);
       });

    return 0;
}
