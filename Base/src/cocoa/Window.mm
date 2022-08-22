//
// Created by Daemyung Jang on 2021/09/04.
//

#include "Base/Window.h"

#include <spdlog/spdlog.h>
#include <SDL_syswm.h>
#include <EGL/egl.h>
#include <AppKit/AppKit.h>

/// 윈도우가 가지고 있는 윈도우 매니저 정보를 반환합니다.
/// \param window 윈도우 매니저 정보를 가지고 있는 윈도우 객체입니다.
/// \return 윈도우 매니저 정보를 반환합니다.
SDL_SysWMinfo sdl_sys_wm_info(SDL_Window *window) {
    SDL_SysWMinfo sys_wm_info;
    SDL_VERSION(&sys_wm_info.version)

    if (!SDL_GetWindowWMInfo(window, &sys_wm_info)) {
        spdlog::error("{}", SDL_GetError());
        throw std::runtime_error("Fail to get a window manager information.");
    }

    return sys_wm_info;
}

int32_t Window::native_display(const Window &window) {
    return EGL_DEFAULT_DISPLAY;
}

void *Window::native_window(const Window &window) {
    NSView *view = [sdl_sys_wm_info(window.window_).info.cocoa.window contentView];
    if (![view wantsLayer]) {
        [view setWantsLayer:YES];
    }
    return (__bridge void *) [view layer];
}

float Window::dpi() const {
    return static_cast<float>([sdl_sys_wm_info(window_).info.cocoa.window backingScaleFactor]);
}
