# 기본 구조 만들기

이제 OpenGL ES를 사용하여 포트폴리오를 만들 준비가 되었습니다. 처음부터 모든 것을 스스로 만들 수도 있지만 저는 다른 사람이 만든 프로그램의 구조를 이해하고 제 것으로 만드는 것도 좋은 학습법 중 하나라고 생각합니다. 때문에 제가 OpenGL ES를 처음 배울 때 큰 도움이 되었던 [OpenGLES 강좌](https://github.com/GraphicsKorea/OpenGLES)의 코드를 바탕으로 저의 포트폴리오를 만들겠습니다.

## 개요
먼저 Base 디렉토리에 **[Window.h](https://github.com/GraphicsKorea/OpenGLES/blob/main/Base/include/Base/Window.h)**, **[Window.cpp](https://github.com/GraphicsKorea/OpenGLES/blob/main/Base/src/Window.cpp)** 와 **[utility.h](https://github.com/GraphicsKorea/OpenGLES/blob/main/Base/include/Base/utility.h)**, **[utility.cpp](https://github.com/GraphicsKorea/OpenGLES/blob/main/Base/src/utility.cpp)** 가 있습니다. 
- **Window.cpp** 는 Window 클래스를 정의하고 있습니다. Window 클래스에서는 SDL 라이브러리를 사용하여 윈도우 생성, 제거, 렌더링 루프, 이벤트 등에 대한 동작을 담당합니다.
- **utility.h** 는 EGL 초기화와 객체 생성, 파괴에 대한 동작을 담당합니다. **utility.cpp** 는 셰이더를 생성하고 프로그램을 생성하는 동작을 담당합니다. 하지만 지금은 사용하지 않고 나중에 사용하게 될 것입니다.
- 마지막으로 08.MakeCurrentEGL 디렉토리의 **[main.cpp](https://github.com/GraphicsKorea/OpenGLES/blob/main/08.MakeCurrentEGL/src/main.cpp)** 에서 작성한 로직을 실행시킵니다.

## 실행 흐름 따라가기

main 문을 실행했을 때 프로그램이 어떻게 동작하는지 단계별로 좀 더 자세히 구조를 파악해보겠습니다.

### 1. app 객체와 window 객체 생성

```cpp
// 08.MakeCurrentEGL/src/main.cpp
...

struct App {
    EGLDisplay display{EGL_NO_DISPLAY};
    EGLConfig config{nullptr};
    EGLContext context{EGL_NO_CONTEXT};
    EGLSurface surface{EGL_NO_SURFACE};
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    // app 객체 생성
    App app{};
    // window 객체 생성
    Window window{{.title = "Current EGL 만들기", .size = {1280, 720}}};

    ...
}
```

main 문이 실행되면 먼저 App 타입의 `app` 객체가 생성됩니다. main문 위에 App 구조체가 정의되어 있습니다. App 구조체는 EGL을 사용하는데 필요한 데이터들을 멤버변수로 가집니다.

그 다음줄에서 `window` 객체를 생성합니다. `window` 객체의 생성자 파라미터로 윈도우에 표시할 제목과 윈도우의 크기를 지정합니다.

```cpp
// Base/src/Window.cpp
...

Window::Window(const Descriptor &descriptor) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        spdlog::error("{}", SDL_GetError());
        throw std::runtime_error("Fail to create Window.");
    }
    
    // 네이티브 윈도우 생성
    window_ = SDL_CreateWindow(descriptor.title.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               descriptor.size.x,
                               descriptor.size.y,
                               SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window_) {
        spdlog::error("{}", SDL_GetError());
        throw std::runtime_error("Fail to create Window.");
    }
}

...
```

`window` 객체가 생성될 때, Window 클래스의 생성자가 호출됩니다. 생성자에서 SDL 라이브러리의 SDL_CreateWindow 로 네이티브 플랫폼에 맞는 윈도우를 생성하고 객체의 맴버 변수로 저장합니다.

### 2. Window 클래스의 run 맴버함수 실행

가장 핵심적인 역할을 하는 `run` 맴버함수의 구조에 대해 알아보겠습니다.
`run` 맴버함수는 4개의 함수를 파리미터로 받고, 역할에 맞는 위치에서 각 함수들을 실행시킵니다.

```cpp
// Base/src/Window.cpp
...

void Window::run(const std::function<void()> &startup,
				 const std::function<void()> &update,
                 const std::function<void()> &render,
                 const std::function<void()> &shutdown) {
    startup();
    SDL_ShowWindow(window_);

    while (process_event()) {
        update();
        render();
    }

    SDL_HideWindow(window_);
    shutdown();
}

...
```

`startup` 함수에서 이전 장에서 정리했던 EGL 초기 설정에 대한 동작이 수행됩니다.
while 문은 윈도우가 닫히기 전까지 `update` 함수와 `render` 함수를 매 프레임마다 반복적으로 실행합니다. 따라서 렌더링 연산들은 while 문의 스코프 안쪽 함수들에서 실행됩니다.
마지막으로 윈도우가 닫히게 되어 while 문을 빠져나가게 된다면 `shutdown` 함수에서 생성된 EGL을 정리합니다.

다시 main문으로 돌아와서

```cpp
// 08.MakeCurrentEGL/src/main.cpp

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    ...

    window.run([&app, &window] { // startup
        app.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (app.display == EGL_NO_DISPLAY) {
            spdlog::error("기본 디스플레이로부터 EGLDisplay를 얻을 수 없습니다.");
            std::terminate();
        }

        EGL_TEST(eglInitialize(app.display, nullptr, nullptr));
        EGL_TEST(eglBindAPI(EGL_OPENGL_ES_API));

        EGLint config_attributes[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_ALPHA_SIZE, 8,
                EGL_DEPTH_SIZE, 16,
                EGL_STENCIL_SIZE, 8,
                EGL_NONE
        };
        EGLint num_config;
        EGL_TEST(eglChooseConfig(app.display, config_attributes, &app.config, 1, &num_config));

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
    }, 
    [] {}, // update
    [&app] { // render
        GL_TEST(glClearColor(0.13f, 0.44f, 0.69f, 1.0f));
        GL_TEST(glClear(GL_COLOR_BUFFER_BIT));

        EGL_TEST(eglSwapBuffers(app.display, app.surface));
    },
    [&app] { // shutdown
        EGL_TEST(eglDestroySurface(app.display, app.surface));
        app.surface = EGL_NO_SURFACE;

        EGL_TEST(eglDestroyContext(app.display, app.context));
        app.context = EGL_NO_CONTEXT;

        if (!eglTerminate(app.display)) {
            spdlog::error("EGL을 종료를 실패하였습니다.");
            std::terminate();
        }
        app.display = EGL_NO_DISPLAY;
    });

    return 0;
}
```

람다 문법을 사용하여 `run` 맴버함수의 `startup`, `render`, `shutdown` 파라미터에 함수를 정의해 줍니다.

### 3. 실행 결과

![image](./images/Screen%20Shot%202022-08-29%20at%204.46.06%20PM.png)

## 참고
- [OpenGLES - GraphicsKorea](https://github.com/GraphicsKorea/OpenGLES)
