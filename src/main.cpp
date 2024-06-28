#include "pch.h"

#include "Windowing.h"
#include "Renderer.h"
#include "Object.h"
#include "Camera.h"

#include "Console.h"
#include <profileapi.h>
#include <winuser.h>

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    LARGE_INTEGER start, end;
    double elapsedTime;

    Console::Create();
    QueryPerformanceFrequency(&freq);

    windowSystem.Init();

    Console::Out("Window Init");   
    Console::Out("Width: ", cfgDefaultWindowWidth);
    Console::Out("Height: ", cfgDefaultWindowHeight);

    windowSystem.SetInstance(hInstance);
    if(window)
        windowSystem.GetMainWindow()->Show(SW_SHOW);

    objectSystem.Init();
    Console::Out("Objects Init");

    rendererSystem.Init(&windowSystem, &objectSystem);
    Console::Out("Renderer Init");

    Console::Out("Renderer - Assigning device context");
    rendererSystem.PrepareDeviceContext();
    Console::Out("Renderer - Initializing buffers: ");
    rendererSystem.InitBuffers();

    Console::Out();

    objectSystem.AddTriangle(Triangle(Vector3(-0.75, -0.5, 0), Vector3(-0.5, 0.5, 0), Vector3(0.5,  0.5, 0)));
    // objectSystem.AddTriangle(Triangle(Vector3(-0.75, -0.5, 0), Vector3(0.5,  0.5, 0), Vector3(0.5,  -0.5, 0)));
    // objectSystem.AddDrawable(Triangle3d(Vector2(-0.75, -0.5), Vector2(0.5,  0.5), Vector2(0.5,  -0.5)));

    Console::Out("Starting Render Loop");

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
                Console::Out("fps: ", 1/elapsedTime, " frametime (Ms): ", (1/(1/elapsedTime))*1000, " frame: ", currentFrame);
            }
        }
    }
    return (int)msg.wParam;
}