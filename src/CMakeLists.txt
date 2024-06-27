add_library(SimpleRendererSrc STATIC
    Core.h
    Math.h
    Math.cpp
    System.h
    Primitives.h
    Primitives.cpp
    Windowing.h
    Windowing.cpp
    Camera.h
    Camera.cpp
    Renderer.h
    Renderer.cpp
    Object.h
    Object.cpp
    Windowing.h
    Windowing.cpp
)

target_include_directories(SimpleRendererSrc PRIVATE ../external/include)