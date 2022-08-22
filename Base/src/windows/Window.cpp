//
// Created by Daemyung Jang on 2021/09/01.
//

#include "Base/Window.h"

#include <spdlog/spdlog.h>
#include <SDL_syswm.h>
#include <Windows.h>

/// �����찡 ������ �ִ� ������ �Ŵ��� ������ ��ȯ�մϴ�.
/// \param window ������ �Ŵ��� ������ ������ �ִ� ������ ��ü�Դϴ�.
/// \return ������ �Ŵ��� ������ ��ȯ�մϴ�.
SDL_SysWMinfo sdl_sys_wm_info(SDL_Window* window) {
    SDL_SysWMinfo sys_wm_info;
    SDL_VERSION(&sys_wm_info.version)

        if (!SDL_GetWindowWMInfo(window, &sys_wm_info)) {
            spdlog::error("{}", SDL_GetError());
            throw std::runtime_error("Fail to get a window manager information.");
        }

    return sys_wm_info;
}

int32_t Window::native_display(const Window& window) {
	return reinterpret_cast<int32_t>(sdl_sys_wm_info(window.window_).info.win.hdc);
}

void* Window::native_window(const Window& window) {
    return sdl_sys_wm_info(window.window_).info.win.window;
}
