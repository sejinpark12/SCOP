/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 18:24:49 by sejpark           #+#    #+#             */
/*   Updated: 2022/09/02 15:59:00 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Daemyung Jang on 2021/10/19.
//

#include <__nullptr>
#include <array>
#include <vector>
#include <spdlog/spdlog.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Base/Window.h>
#include <Base/utility.h>
#include <Base/Shader.h>
#include <Base/Model.h>

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
};

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const float fov = 45.0f;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    App app{};
    Window window{{.title = "SCOP", .size = {SCR_WIDTH, SCR_HEIGHT}}};

    window.run([&app, &window] {
            startup(app,window);
            printAPIInfo(app);

            GL_TEST(glEnable(GL_DEPTH_TEST));
            Shader shader({home() / "SCOP/res/triangle.vert",
                           home() / "SCOP/res/unlit.frag"});
            app.program = shader.getProgramId();
            app.model = new Model(home() / "SCOP/res/objects/teapot.obj");
       },
       [] {},
       [&app, &window] {
           GL_TEST(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
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
            
           app.model->Draw(app.program);

           GL_TEST(glBindVertexArray(0));
           GL_TEST(glUseProgram(0));

           EGL_TEST(eglSwapBuffers(app.display, app.surface));
       },
       [&app] {
           GL_TEST(glDeleteProgram(app.program));
           app.program = 0;

           app.model->clearModel();
           delete app.model;
           shutdown(app);
       });

    return 0;
}
