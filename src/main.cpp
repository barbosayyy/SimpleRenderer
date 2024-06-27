#include "pch.h"

#include "Windowing.h"
#include "Renderer.h"
#include "Object.h"
#include "Camera.h"

#include <iostream>
#include <profileapi.h>

// Debug
bool window = 1;
bool queryPerformance = 1;
bool fps = 1;

LARGE_INTEGER freq;
int currentFrame;

WindowSystem windowSystem;
RendererSystem rendererSystem;
ObjectSystem objectSystem;

//
// -- TODO: - Rasterization algorithm       
//          - Z-buffer and depth check (near, far)
//          - Threads
//

void CreateConsole() {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
    std::ios::sync_with_stdio();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    LARGE_INTEGER start, end;
    double elapsedTime;

    CreateConsole();
    QueryPerformanceFrequency(&freq);

    windowSystem.Init();
    std::cout << "Window Init" << std::endl;
    std::cout << "Width: " << cfgDefaultWindowWidth << std::endl;
    std::cout << "Height: " << cfgDefaultWindowHeight << std::endl;
    windowSystem.SetInstance(hInstance);
    if(window)
        windowSystem.GetMainWindow()->Show(SW_SHOW);

    objectSystem.Init();
    std::cout << "Objects Init" << std::endl;

    rendererSystem.Init(&windowSystem, &objectSystem);
    std::cout << "Renderer Init" << std::endl;

    rendererSystem.PrepareDeviceContext();
    std::cout << "Renderer - Assigned device context" << std::endl;
    std::cout << "Renderer - Initializing buffers: ";
    rendererSystem.InitBuffers();

    std::cout << std::endl;

    objectSystem.AddTriangle(Triangle(Vector3(-0.75, -0.5, 0), Vector3(-0.5, 0.5, 0), Vector3(0.5,  0.5, 0)));
    // objectSystem.AddDrawable(Triangle(Vector2(-0.75, -0.5), Vector2(0.5,  0.5), Vector2(0.5,  -0.5)));
    // objectSystem.AddDrawable(Triangle3d(Vector2(-0.75, -0.5), Vector2(0.5,  0.5), Vector2(0.5,  -0.5)));

     std::cout << "Game Loop" << std::endl;

    MSG msg = {};
    while(msg.message != WM_QUIT && windowSystem.MainWindowNotClosed() == true){
        if(queryPerformance)
            QueryPerformanceCounter(&start);

        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        rendererSystem.ResizeWindow();

        rendererSystem.Render();
        
        if(queryPerformance){
            QueryPerformanceCounter(&end);
            elapsedTime = static_cast<double>(end.QuadPart - start.QuadPart) / freq.QuadPart;
            currentFrame += 1/elapsedTime;
            if(fps){
                std::cout << "fps: " << 1/elapsedTime << " frametime (Ms): " << (1/(1/elapsedTime))*1000 << " frame: " << currentFrame << std::endl;
            }
        }
    }
    return (int)msg.wParam;
}