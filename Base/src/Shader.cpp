/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:16:40 by sejpark           #+#    #+#             */
/*   Updated: 2022/08/31 19:18:09 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base/Shader.h"

Shader::Shader(const std::array<std::filesystem::path, 2> &paths)
	: programId_(createGraphicsPipeline(paths)) {}

Shader::~Shader() {}

GLuint Shader::getProgramId() const {
	return programId_;
}

GLenum Shader::shaderType(const std::filesystem::path &path) {
	if (path.extension() == ".vert" || path.extension() == ".vs")
		return GL_VERTEX_SHADER;
	if (path.extension() == ".frag" || path.extension() == ".fs")
		return GL_FRAGMENT_SHADER;

	spdlog::error("Use of unknown extension '{}'.", path.extension().string());
	throw std::runtime_error("Fail to cast the shader type.");
}

std::string Shader::readFile(const std::filesystem::path &path) {
	std::ifstream fin;
	fin.open(path, std::ios::binary | std::ios::ate);

	if (!fin.is_open()) {
		spdlog::error("Fail to open {}.", path.string());
		throw std::runtime_error("Fail to read file.");
	}

	const std::streamsize size = fin.tellg();
	if (!size) {
		spdlog::error("{} is empty.", path.string());
		throw std::runtime_error("Fail to read file.");
	}

	std::string contents(size, '\0');
	fin.seekg(std::ios::beg);

	if (!fin.read(contents.data(), size)) {
		spdlog::error("Fail to read {}.", path.string());
		throw std::runtime_error("Fail to read file.");
	}

	return contents;
}

GLuint Shader::createShader(GLenum shaderType, const std::string &source) {
	GLuint shader = glCreateShader(shaderType);
	if (!shader) {
		spdlog::error("Fail to create a shader.");
		throw std::runtime_error("Fail to create a shader.");
	}

	const char *data = source.data();
	GL_TEST(glShaderSource(shader, 1, &data, nullptr));
	GL_TEST(glCompileShader(shader));

	GLint status;
	GL_TEST(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));

	if (!status) {
		GLint length;
		GL_TEST(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));

		if (length) {
			std::string log(length, '\n');
			GL_TEST(glGetShaderInfoLog(shader, length, nullptr, log.data()));
			spdlog::error("{}.", log);
		}

		GL_TEST(glDeleteShader(shader));
		throw std::runtime_error("Fail to create a shader.");
	}
	return shader;
}

GLuint Shader::createShader(const std::filesystem::path &path) {
	return createShader(shaderType(path), readFile(path));
}

GLuint Shader::createGraphicsPipeline(const std::array<std::filesystem::path, 2> &paths) {
	GLuint program = glCreateProgram();
	if (!program) {
		spdlog::error("Fail to create a program.");
		throw std::runtime_error("Fail to create a program.");
	}

	const GLuint shaders[] = {
		createShader(paths[0]),
		createShader(paths[1])
	};
	
	GL_TEST(glAttachShader(program, shaders[0]));
	GL_TEST(glAttachShader(program, shaders[1]));
	GL_TEST(glLinkProgram(program));

	GLint status;
	GL_TEST(glGetProgramiv(program, GL_LINK_STATUS, &status));

	if (!status) {
		GLint length;
		GL_TEST(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

		if (length) {
			std::string log(length, '\n');
			GL_TEST(glGetProgramInfoLog(program, length, nullptr, log.data()));
			spdlog::error("Err to link a program.\n{}.", log);
		}

		GL_TEST(glDeleteProgram(program));
		throw std::runtime_error("Fail to create a program.");
	}

	GL_TEST(glDeleteShader(shaders[0]));
	GL_TEST(glDeleteShader(shaders[1]));

	return program;
}
