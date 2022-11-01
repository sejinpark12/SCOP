//
// Created by Daemyung Jang on 2021/10/19.
// Updated by Sejin Park on 2022/10/27.
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
#include <Base/App.h>
#include <Base/Window.h>
#include <Base/utility.h>
#include <Base/Shader.h>
//#include <Base/Model.h>
//#include <Base/GeometricShapes/Sphere.h>
#include <Base/Objects.h>
#include <Base/Ui.h>
#include <Base/Uniforms.h>

//struct App {
//    EGLDisplay display{EGL_NO_DISPLAY};
//    EGLConfig config{nullptr};
//    EGLContext context{EGL_NO_CONTEXT};
//    EGLSurface surface{EGL_NO_SURFACE};
//    std::vector<GLuint> programs;
//    GLint model_location{0};
//    GLint view_location{0};
//    GLint projection_location{0};
//    Ui *ui{nullptr};
//    std::vector<Model*> models;
//    std::vector<Sphere*> spheres;
//};

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const float fov = 45.0f;

Uniforms uniforms{};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    App app{};
    Window window{{.title = "SCOP", .size = {SCR_WIDTH, SCR_HEIGHT}}};

    window.run([&app, &window] {
            startup(app, window);
            printAPIInfo(app);

            GL_TEST(glEnable(GL_DEPTH_TEST));
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

            app.ui = new Ui(window);
       },
       [] {},
       [&app, &window] {
           //app.ui->drawUi(window.get_camera(), uniforms, app.models, app.spheres);
           app.ui->drawUi(uniforms, app.objects);

           GL_TEST(glClearColor(uniforms.backGroundColor.r,
                                uniforms.backGroundColor.g,
                                uniforms.backGroundColor.b,
                                uniforms.backGroundColor.a));
           GL_TEST(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

           GL_TEST(glViewport(0, 0, window.size().x * window.dpi(), 
                       window.size().y * window.dpi()));

           GL_TEST(glUseProgram(app.programs[uniforms.selectLight]));
           
           glm::mat4 projection = glm::perspective(glm::radians(fov),
                   static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
                   0.001f, 1000.0f) ;
           app.projection_location = glGetUniformLocation(app.programs[uniforms.selectLight], "projection");
           GL_TEST(glUniformMatrix4fv(app.projection_location, 1, GL_FALSE,
                       &projection[0][0]));
           glm::mat4 view = window.get_camera().getViewMatrix();
           app.view_location = glGetUniformLocation(app.programs[uniforms.selectLight], "view");
           GL_TEST(glUniformMatrix4fv(app.view_location, 1, GL_FALSE, &view[0][0]));
           glm::mat4 model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
           app.model_location = glGetUniformLocation(app.programs[uniforms.selectLight], "model");
           GL_TEST(glUniformMatrix4fv(app.model_location, 1, GL_FALSE, &model[0][0]));

           GLint lightDir_location{0};
           glm::vec4 lightDirection = glm::normalize(uniforms.lightDir);
           lightDir_location = glGetUniformLocation(app.programs[uniforms.selectLight], "l_dir");
           GL_TEST(glUniform4fv(lightDir_location, 1, glm::value_ptr(lightDirection)));

           GLint lightPos_location{0};
           lightPos_location = glGetUniformLocation(app.programs[uniforms.selectLight], "l_pos");
           GL_TEST(glUniform4fv(lightPos_location, 1, glm::value_ptr(uniforms.lightPos)));

           GLint camPos_location{0};
           glm::vec4 camPosition = glm::vec4(window.get_camera().getPosition(), 1.0);
           camPos_location = glGetUniformLocation(app.programs[uniforms.selectLight], "campos");
           GL_TEST(glUniform4fv(camPos_location, 1, glm::value_ptr(camPosition)));

           GLint spotDir_location{0};
           spotDir_location = glGetUniformLocation(app.programs[uniforms.selectLight], "l_spotDir");
           GL_TEST(glUniform4fv(spotDir_location, 1, glm::value_ptr(uniforms.spotDir)));
//           SPDLOG_INFO("spotDirection: {}, {}, {}, {}", spotDirection.x, spotDirection.y, spotDirection.z, spotDirection.w);

           GLint spotCutOff_location{0};
           float spotCutOff = glm::cos(glm::radians(uniforms.spotCutOffAngle));
           spotCutOff_location = glGetUniformLocation(app.programs[uniforms.selectLight], "l_spotCutOff");
           GL_TEST(glUniform1fv(spotCutOff_location, 1, &spotCutOff));

           GLint diffuse_location{0};
           diffuse_location = glGetUniformLocation(app.programs[uniforms.selectLight], "diffuse");
           GL_TEST(glUniform4fv(diffuse_location, 1, glm::value_ptr(uniforms.diffuse)));

           GLint ambient_location{0};
           ambient_location = glGetUniformLocation(app.programs[uniforms.selectLight], "ambient");
           GL_TEST(glUniform1fv(ambient_location, 1, &uniforms.ambient));

           GLint shininess_location{0};
           shininess_location = glGetUniformLocation(app.programs[uniforms.selectLight], "shininess");
           GL_TEST(glUniform1fv(shininess_location, 1, &uniforms.shininess));

           GLint specular_location{0};
           specular_location = glGetUniformLocation(app.programs[uniforms.selectLight], "specular");
           GL_TEST(glUniform4fv(specular_location, 1, glm::value_ptr(uniforms.specular)));
            
           for (size_t i = 0; i < app.objects.models.size(); i++)
               app.objects.models[i]->Draw(app.programs[uniforms.selectLight]);

           for (size_t i = 0; i < app.objects.spheres.size(); i++) {
               //app.objects.spheres[i]->draw(app.programs[uniforms.selectLight]);
               app.objects.spheres[i]->draw();
           }

           GL_TEST(glBindVertexArray(0));
           GL_TEST(glUseProgram(0));

           Ui::render();

           EGL_TEST(eglSwapBuffers(app.display, app.surface));
       },
       [&app] {
           for (size_t i = 0; i < app.programs.size(); i++)
               GL_TEST(glDeleteProgram(app.programs[i]));
           std::vector<GLuint>().swap(app.programs);


           for (size_t i = 0; i < app.objects.models.size(); i++) {
               app.objects.models[i]->clearModel();
               delete app.objects.models[i];
               app.objects.models.pop_back();
           }

           for (size_t i = 0; i < app.objects.spheres.size(); i++) {
               delete app.objects.spheres[i];
           }

           delete app.ui;

           shutdown(app);
       });

    return 0;
}
