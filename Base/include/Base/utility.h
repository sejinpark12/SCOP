//
// Created by Daemyung Jang on 2021/09/12.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <array>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "Window.h"

#define STRING(x) #x

#ifndef NDEBUG
#define EGL_TEST(function) do { \
    if (!function) { \
        EGLint error = eglGetError(); \
        if (error != EGL_SUCCESS) { \
            spdlog::error("{} with 0x{:x}.", STRING(function), error); \
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
        spdlog::error("{} with 0x{:x}.", STRING(function), error); \
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
        spdlog::error("기본 디스플레이로부터 EGLDisplay를 얻을 수 없습니다.");
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
        spdlog::error("EGLContext를 생성할 수 없습니다.");
        std::terminate();
    }

    app.surface = eglCreateWindowSurface(app.display, app.config, Window::native_window(window), nullptr);
    if (app.surface == EGL_NO_SURFACE) {
        spdlog::error("EGLSurface를 생성할 수 없습니다.");
        std::terminate();
    }

    EGL_TEST(eglMakeCurrent(app.display, app.surface, app.surface, app.context));
}

/// EGL 객체 파괴 및 종료를 합니다.
/// \tparam App EGL 객체를 저장할 수 있는 타입입니다.
/// \param app App 객체입니다.
template<typename App>
void shutdown(App &app) {
    EGL_TEST(eglMakeCurrent(app.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));

    EGL_TEST(eglDestroySurface(app.display, app.surface));
    app.surface = EGL_NO_SURFACE;

    EGL_TEST(eglDestroyContext(app.display, app.context));
    app.context = EGL_NO_CONTEXT;

    if (!eglTerminate(app.display)) {
        spdlog::error("EGL을 종료를 실패하였습니다.");
        std::terminate();
    }
    app.display = EGL_NO_DISPLAY;
}

/// HOME 경로를 반환합니다.
/// \return HOME 경로를 반환합니다.
std::filesystem::path home();

/// 셰이더를 생성합니다.
/// \param path 셰이더 소스 파일의 경로입니다.
/// \return 생성된 셰이더를 반환합니다.
GLuint create_shader(const std::filesystem::path &path);

/// 프로그램을 생성합니다.
/// \param paths 버텍스, 프레그먼트 셰이더 소스 파일의 경로입니다.
/// \return 생성된 프로그램을 반환합니다.
GLuint create_graphics_pipeline(const std::array<std::filesystem::path, 2> &paths);

#endif //UTILITY_H
