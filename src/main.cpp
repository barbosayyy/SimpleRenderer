#include "pch.h"

#include "InputSystem.h"
#include "WindowSystem.h"
#include "RendererSystem.h"
#include "Camera.h"

#include "Console.h"
#include <profileapi.h>
#include <winuser.h>

// Debug
bool window = 1;
bool queryPerformance = 0;
bool fps = 0;
bool pause = 0;

LARGE_INTEGER freq;
SHORT var1;
int currentFrame;

WindowSystem windowSystem;
RendererSystem rendererSystem;
InputSystem inputSystem;

//
// -- TODO: - improve rendering algorithm for better performance 
//          - improve task queueing
//          - add safeties on renderer interruptions
//          - shading
//

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    LARGE_INTEGER start, end;
    double elapsedTime;

    Console::Create();
    QueryPerformanceFrequency(&freq);

    inputSystem.Init();

    windowSystem.Init(hInstance);
    if(window)
        windowSystem.GetMainWindow()->Show(SW_SHOW);

    rendererSystem.Init(&windowSystem);
    rendererSystem.GetMainCamera().RegisterInputSystem(&inputSystem);

    Console::Out();

    rendererSystem.AddTriangle(Triangle(Vector3(-1.0, 1.0, 0), Vector3(-1.0, -1.0, 0), Vector3(1.0,  -1.0, 0)));
    rendererSystem.AddTriangle(Triangle(Vector3(-1.0, 0.5, -1), Vector3(0.5, -1.0, 2.0), Vector3(1.0,  0.5, -1)));
    
    Console::Out("Starting Render Loop");

    MSG msg = {};
    while(msg.message != WM_QUIT && windowSystem.MainWindowNotClosed() == true){
        if(queryPerformance)
            QueryPerformanceCounter(&start);

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
            currentFrame += 1/elapsedTime;
            if(fps){
                Console::Out("fps: ", 1/elapsedTime, " frametime (Ms): ", (1/(1/elapsedTime))*1000, " frame: ", currentFrame);
            }
        }
    }
    return (int)msg.wParam;
}