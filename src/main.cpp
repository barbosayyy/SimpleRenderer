#include "pch.h"

#include "Core.h"
#include "InputSystem.h"
#include "WindowSystem.h"
#include "RendererSystem.h"
#include "Camera.h"

#include "Console.h"
#include <profileapi.h>
#include <winuser.h>

//
// -- TODO: - improve rendering algorithm for better performance 
//          - improve task queueing
//          - shading
//

// Debug
bool window = 1;
bool queryPerformance = 0;
bool fps = 0;
bool pause = 0;

bool shouldClose = 0;
LARGE_INTEGER freq;
int currentFrame;

WindowSystem windowSystem;
RendererSystem rendererSystem;
InputSystem inputSystem;

void InitializeInput(){
    inputSystem.Init();
}

void InitializeWindow(HINSTANCE hInstance){
    windowSystem.Init(hInstance);
    if(window){
        windowSystem.GetMainWindow()->Show(SW_SHOW);
        windowSystem.MoveWindow(1080, 0);
    }
}

void InitializeRenderer(){
    rendererSystem.Init(&windowSystem);
    rendererSystem.GetMainCamera().RegisterInputSystem(&inputSystem);
}

void Start(){
    rendererSystem.AddTriangle(Triangle(Vector3(-1.0, 1.0, 0), Vector3(-1.0, -1.0, 0), Vector3(1.0,  -1.0, 0)));
    rendererSystem.AddTriangle(Triangle(Vector3(-1.0, 1.0, 0), Vector3(1.0, -1.0, 0), Vector3(1.0,  1.0, 0)));
    rendererSystem.AddTriangle(Triangle(Vector3(1.0, 1.0, 0), Vector3(1.0, -1.0, 0), Vector3(1.0,  -1.0, -2.0)));
    rendererSystem.AddTriangle(Triangle(Vector3(1.0, 1.0, 0), Vector3(1.0, -1.0, -2.0), Vector3(1.0,  1.0, -2.0)));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    LARGE_INTEGER start, end;
    double elapsedTime;

    #ifdef EXTERNAL_CONSOLE
    Console::Create();
    #endif

    QueryPerformanceFrequency(&freq);

    InitializeInput();
    InitializeWindow(hInstance);
    InitializeRenderer();   

    Start();
    MSG msg = {};
    while(msg.message != WM_QUIT || shouldClose == false){
        if(queryPerformance){
            QueryPerformanceCounter(&start);
        }

        // Win32 Messages
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        inputSystem.ProcessInput();

        if(!pause){
            rendererSystem.Render();
        }

        if(queryPerformance){
            QueryPerformanceCounter(&end);
            elapsedTime = static_cast<double>(end.QuadPart - start.QuadPart) / freq.QuadPart;
            rendererSystem.GetCurrentFrame() += 1/elapsedTime;
            if(fps){
                Console::Out("fps: ", 1/elapsedTime, " frametime (Ms): ", (1/(1/elapsedTime))*1000, " frame: ", currentFrame);
            }
        }
    }
    rendererSystem.Shutdown();
    windowSystem.Shutdown();
    inputSystem.Shutdown();
    return (int)msg.wParam;
}
