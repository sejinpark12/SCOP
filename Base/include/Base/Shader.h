/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 16:41:05 by sejpark           #+#    #+#             */
/*   Updated: 2022/09/02 15:53:47 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_H
#define SHADER_H

#include <array>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include <GLES3/gl3.h>
#include <Base/utility.h>

/// 셰이더 클래스
class Shader {
public:
    /// 생성자
    /// \param paths 버텍스, 프레그먼트 셰이더 소스 파일의 경로입니다.
    explicit Shader(const std::array<std::filesystem::path, 2> &paths);

    /// 소멸자
    ~Shader();

    /// 프로그램을 반환합니다.
    /// \return programId_ 맴버변수입니다.
    [[nodiscard]] GLuint getProgramId() const;

private:
    /// 셰이더 소스 파일의 타입을 판별합니다.
    /// \param path 셰이더 소스 파일의 경로입니다.
    /// \return 셰이더 타입을 반환합니다.
    GLenum shaderType(const std::filesystem::path &path);

    /// 셰이더 소스 파일을 읽습니다.
    /// \param path 셰이더 소스 파일의 경로입니다.
    /// \return 셰이더 소스 파일에서 읽은 셰이더 코드를 반환합니다.
    std::string readFile(const std::filesystem::path &path);

    /// 셰이더를 생성합니다.
    /// \param shaderType 셰이더 소스 파일의 타입입니다.
    /// \param source 셰이더 소스 파일에서 읽은 문자열입니다.
    /// \return 생성된 셰이더를 반환합니다.
    GLuint createShader(GLenum shaderType, const std::string &source);

    /// 셰이더를 생성합니다.
    /// \param path 셰이더 소스 파일의 경로입니다.
    /// \return 생성된 셰이더를 반환합니다.
    GLuint createShader(const std::filesystem::path &path);

    /// 프로그램을 생성합니다.
    /// \param paths 버텍스, 프레그먼트 셰이더 소스 파일의 경로입니다.
    /// \return 생성된 프로그램을 반환합니다.
    GLuint createGraphicsPipeline(const std::array<std::filesystem::path, 2> &paths);

private:
    GLuint programId_;
};

#endif ///SHADER_H
