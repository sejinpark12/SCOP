//
// Created by Daemyung Jang on 2021/09/12.
// Updated by Sejin Park on 2022/09/18.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include "Window.h"
#include "Ui.h"

#define STRING(x) #x

#ifndef NDEBUG
#define EGL_TEST(function) do { \
    if (!function) { \
        EGLint error = eglGetError(); \
        if (error != EGL_SUCCESS) { \
            SPDLOG_ERROR("{} with 0x{:x}.", STRING(function), error); \
            throw std::runtime_error("Err to call EGL function."); \
        } \
    } \
} while(false)
#else
#define EGL_TEST(function) function
#endif

#ifndef NDEBUG
#define GL_TEST(function) do { \
    function; \
    GLenum error = glGetError(); \
    if (error != GL_NO_ERROR) { \
        SPDLOG_ERROR("{} with 0x{:x}.", STRING(function), error); \
        throw std::runtime_error("Err to call GL function."); \
    } \
} while(false)
#else
#define GL_TEST(function) function
#endif

#define GL_OFFSETOF(type, field) reinterpret_cast<void*>(offsetof(type, field))

/// 컨테이너의 바이트 크기를 반환합니다.
/// \tparam Container 컨테이너 타입입니다.
/// \param container 컨테이너 객체입니다.
/// \return 컨테이너의 바이트 크기입니다.
template<typename Container>
inline int64_t byte_size(const Container& container) {
    return sizeof(typename Container::value_type) * container.size();
}

/// OpenGL ES를 위한 EGL 초기화 및 객체 생성을 합니다.
/// \tparam App EGL 객체를 저장할 수 있는 타입입니다.
/// \param app App 객체입니다.
/// \param window Window 객체입니다.
template<typename App>
void startup(App &app, Window &window) {
    app.display = eglGetDisplay(Window::native_display(window));
    if (app.display == EGL_NO_DISPLAY) {
        SPDLOG_ERROR("기본 디스플레이로부터 EGLDisplay를 얻을 수 없습니다.");
        std::terminate();
    }

    EGL_TEST(eglInitialize(app.display, nullptr, nullptr));
    EGL_TEST(eglBindAPI(EGL_OPENGL_ES_API));

    EGLint num_config;
    EGL_TEST(eglChooseConfig(app.display, nullptr, &app.config, 1, &num_config));

    EGLint context_attributes[] = {
            EGL_CONTEXT_MAJOR_VERSION, 3,
            EGL_NONE
    };
    app.context = eglCreateContext(app.display, app.config, EGL_NO_CONTEXT, context_attributes);
    if (app.context == EGL_NO_CONTEXT) {
        SPDLOG_ERROR("EGLContext를 생성할 수 없습니다.");
        std::terminate();
    }

    app.surface = eglCreateWindowSurface(app.display, app.config, Window::native_window(window), nullptr);
    if (app.surface == EGL_NO_SURFACE) {
        SPDLOG_ERROR("EGLSurface를 생성할 수 없습니다.");
        std::terminate();
    }

    EGL_TEST(eglMakeCurrent(app.display, app.surface, app.surface, app.context));

    app.ui = new Ui(window);
}

/// EGL 객체 파괴 및 종료를 합니다.
/// \tparam App EGL 객체를 저장할 수 있는 타입입니다.
/// \param app App 객체입니다.
template<typename App>
void shutdown(App &app) {
    delete app.ui;

    EGL_TEST(eglMakeCurrent(app.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));

    EGL_TEST(eglDestroySurface(app.display, app.surface));
    app.surface = EGL_NO_SURFACE;

    EGL_TEST(eglDestroyContext(app.display, app.context));
    app.context = EGL_NO_CONTEXT;

    if (!eglTerminate(app.display)) {
        SPDLOG_ERROR("EGL을 종료를 실패하였습니다.");
        std::terminate();
    }
    app.display = EGL_NO_DISPLAY;
}

/// HOME 경로를 반환합니다.
/// \return HOME 경로를 반환합니다.
std::filesystem::path home();

template<typename App>
void printAPIInfo(App &app) {
    SPDLOG_INFO("EGL Version: {}", eglQueryString(app.display, EGL_VERSION));
    SPDLOG_INFO("GL Vendor: {}", (const char*)glGetString(GL_VENDOR));
    SPDLOG_INFO("GL Renderer: {}", (const char*)glGetString(GL_RENDERER));
    SPDLOG_INFO("GL Version: {}", (const char*)glGetString(GL_VERSION));
    SPDLOG_INFO("GLSL Version: {}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

#endif //UTILITY_H
