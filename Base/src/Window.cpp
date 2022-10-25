//
// Created by Daemyung Jang on 2021/09/01.
//

#include "Base/Window.h"
#include "Base/Ui.h"

#include <stdexcept>
#include <spdlog/spdlog.h>
#include <SDL_syswm.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <iostream>

Window::Window(const Descriptor &descriptor) : input_(descriptor.size.x / 2, descriptor.size.y / 2) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SPDLOG_ERROR("{}", SDL_GetError());
        throw std::runtime_error("Fail to create Window.");
    }

    window_ = SDL_CreateWindow(descriptor.title.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               descriptor.size.x,
                               descriptor.size.y,
                               SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window_) {
        SPDLOG_ERROR("{}", SDL_GetError());
        throw std::runtime_error("Fail to create Window.");
    }
}

Window::~Window() {
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    SDL_Quit();
}

void Window::run(const std::function<void()> &startup, const std::function<void()> &update,
                 const std::function<void()> &render, const std::function<void()> &shutdown) {
    int xpos, ypos;
    /// 현재 프레임의 시작 시간
    float currentFrame;
    /// 이전 프레임의 시작 시간
    float lastFrame = 0.0f;
    /// 프레임을 완료하는데 걸린 시간
    float deltaTime = 0.0f;
    /// 맨 처음 프레임의 마우스 커서
    bool firstMouse = true;

    startup();
    SDL_ShowWindow(window_);

    while (process_event()) {
        Ui::newFrame();

        currentFrame = SDL_GetTicks();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (!Ui::isCaptureMouse()) {
            const Uint8 *state = SDL_GetKeyboardState(nullptr);
            input_.processKeyboard(state, deltaTime, camera_);

            if (firstMouse) {
                glm::ivec2 size = this->size();
                SDL_WarpMouseInWindow(window_, size.x / 2, size.y / 2);
                firstMouse = false;
            }
            Uint32 mouseBtns = SDL_GetMouseState(&xpos, &ypos);
            input_.processMouse(mouseBtns, xpos, ypos, camera_);
        }

        camera_.updateCameraVectors();	

        update();
        render();

        Ui::render();
    }

    SDL_HideWindow(window_);
    shutdown();
}

void Window::stop() {
    SDL_Event event{};
    event.window.windowID = SDL_GetWindowID(window_);
    event.window.event = SDL_WINDOWEVENT_CLOSE;
    SDL_PushEvent(&event);
}

std::string Window::title() const {
    return SDL_GetWindowTitle(window_);
}

void Window::title(const std::string &title) {
    SDL_SetWindowTitle(window_, title.c_str());
}

glm::ivec2 Window::size() const {
    glm::ivec2 size;
    SDL_GetWindowSize(window_, &size.x, &size.y);
    return size;
}

void Window::size(const glm::ivec2 &size) {
    SDL_SetWindowSize(window_, size.x, size.y);
}

bool Window::process_event() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (should_close_window(event)) {
            return false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int xpos, ypos;
            SDL_GetMouseState(&xpos, &ypos);
            input_.setLastMousePos(xpos, ypos);
        }
        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT) {
            input_.setMouseRBtnDown(false);
        }
        Ui::processEvent(&event);
        if (event.type == SDL_QUIT)
            return false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window_))
            return true;
    }
    return true;
}

bool Window::should_close_window(const SDL_Event &event) const {
    return event.window.windowID == SDL_GetWindowID(window_) && event.window.event == SDL_WINDOWEVENT_CLOSE;
}

SDL_Window *Window::getWindow() const {
    return window_;
}

Camera &Window::get_camera() {
    return camera_;
}
