/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 18:24:49 by sejpark           #+#    #+#             */
/*   Updated: 2022/08/31 19:50:27 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Daemyung Jang on 2021/10/19.
//

#include <array>
#include <vector>
#include <spdlog/spdlog.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <Base/Window.h>
#include <Base/utility.h>
#include <Base/Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

struct App {
    EGLDisplay display{EGL_NO_DISPLAY};
    EGLConfig config{nullptr};
    EGLContext context{EGL_NO_CONTEXT};
    EGLSurface surface{EGL_NO_SURFACE};
	GLuint vertex_array{0};
	GLuint vertex_buffer{0};
	GLuint index_buffer{0};
	GLuint program{0};
	GLint model_location{0};
	GLint view_location{0};
	GLint projection_location{0};
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
};

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const float fov = 45.0f;

constexpr std::array<Vertex, 4 * 6> vertices = {
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}},
    Vertex{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    Vertex{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    Vertex{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},

    Vertex{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}},
    Vertex{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
    Vertex{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
    Vertex{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},

    Vertex{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}},
    Vertex{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    Vertex{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},

    Vertex{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}},
    Vertex{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    Vertex{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    Vertex{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},

    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}},
    Vertex{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    Vertex{{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
    Vertex{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},

    Vertex{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}},
    Vertex{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    Vertex{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
    Vertex{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}
};

constexpr std::array<uint16_t, 6 * 6> indices = {
    0, 1, 3, 2, 3, 1,
    4, 5, 7, 6, 7, 5,
    8, 9, 11, 10, 11, 9,
    12, 13, 15, 14, 15, 13,
    16, 17, 19, 18, 19, 17,
    20, 21, 23, 22, 23, 21
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    App app{};
    Window window{{.title = "SCOP", .size = {SCR_WIDTH, SCR_HEIGHT}}};

    window.run([&app, &window] {
			startup(app,window);
			printAPIInfo(app);

			GL_TEST(glEnable(GL_DEPTH_TEST));
			GL_TEST(glGenBuffers(1, &app.vertex_buffer));
			GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, app.vertex_buffer));
			GL_TEST(glBufferData(GL_ARRAY_BUFFER, byte_size(vertices), vertices.data(),
						GL_STATIC_DRAW));
			GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, 0));

			GL_TEST(glGenBuffers(1, &app.index_buffer));
			GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app.index_buffer));
			GL_TEST(glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_size(indices),
						indices.data(), GL_STATIC_DRAW));
			GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

			GL_TEST(glGenVertexArrays(1, &app.vertex_array));
			GL_TEST(glBindVertexArray(app.vertex_array));
			GL_TEST(glBindBuffer(GL_ARRAY_BUFFER, app.vertex_buffer));
			GL_TEST(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app.index_buffer));
			GL_TEST(glEnableVertexAttribArray(0));
			GL_TEST(glEnableVertexAttribArray(1));
			GL_TEST(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						GL_OFFSETOF(Vertex, position)));
			GL_TEST(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						GL_OFFSETOF(Vertex, color)));
			GL_TEST(glBindVertexArray(0));

			Shader shader({home() / "SCOP/res/triangle.vert",
						   home() / "SCOP/res/unlit.frag"});
			app.program = shader.getProgramId();
	   },
	   [] {},
	   [&app, &window] {
		   GL_TEST(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
		   GL_TEST(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		   GL_TEST(glViewport(0, 0, window.size().x * window.dpi(), 
					   window.size().y * window.dpi()));

		   GL_TEST(glUseProgram(app.program));
		   GL_TEST(glBindVertexArray(app.vertex_array));
		   
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
		   GL_TEST(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr));

		   GL_TEST(glBindVertexArray(0));
		   GL_TEST(glUseProgram(0));

		   EGL_TEST(eglSwapBuffers(app.display, app.surface));
	   },
	   [&app] {
		   GL_TEST(glDeleteProgram(app.program));
		   app.program = 0;

		   GL_TEST(glDeleteVertexArrays(1, &app.vertex_array));
		   app.vertex_array = 0;

		   GL_TEST(glDeleteBuffers(1, &app.vertex_buffer));
		   app.vertex_buffer = 0;

		   GL_TEST(glDeleteBuffers(1, &app.index_buffer));
		   app.index_buffer = 0;

		   shutdown(app);
	   });

    return 0;
}
