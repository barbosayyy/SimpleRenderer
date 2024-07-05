add_library(SimpleRendererSrc STATIC
    Core.h
    Math.h
    Math.cpp
    Console.h
    Console.cpp
    System.h
    WndProc.h
    WndProc.cpp
    Primitives.h
    Primitives.cpp
    InputSystem.h
    InputSystem.cpp
    Window.h
    Window.cpp
    WindowSystem.h
    WindowSystem.cpp
    Camera.h
    Camera.cpp
    RenderThreadPool.h
    RenderThreadPool.cpp
    IRendererSystem.h
    RendererSystem.h
    RendererSystem.cpp
)

target_include_directories(SimpleRendererSrc PRIVATE ../external/include)