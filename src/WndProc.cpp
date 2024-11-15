#include "WndProc.h"
#include "Console.h"
#include "WindowSystem.h"
#include "RendererSystem.h"
#include "glm/glm.hpp"
#include "windowsx.h"

// Window Message Procedure

extern WindowSystem windowSystem;
extern RendererSystem rendererSystem;
extern bool pause;
extern bool shouldClose;

LRESULT CALLBACK WndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg){
        case WM_PAINT:
        break;
        case WM_DESTROY:
            if(shouldClose == false){
                PostQuitMessage(0);
                shouldClose = true;
            }
        break;
        case WM_EXITSIZEMOVE: {
            RECT rect;
            GetClientRect(wnd, &rect);
            windowSystem.GetWindowByHandle(wnd)->_newWidth = rect.right - rect.left;
            windowSystem.GetWindowByHandle(wnd)->_newHeight = rect.bottom - rect.top;
        }
        case WM_ENTERSIZEMOVE: {

            }
        break;
        case WM_SIZE:{
            Console::Out(wParam);
            if(wParam == SIZE_RESTORED){

            }
            else if(wParam == SIZE_MINIMIZED){

            }
            else if(wParam == SIZE_MAXIMIZED){
                RECT rect;
                GetClientRect(wnd, &rect);
                windowSystem.GetWindowByHandle(wnd)->_newWidth = rect.right - rect.left;
                windowSystem.GetWindowByHandle(wnd)->_newHeight = rect.bottom - rect.top;
            }
        }
        case WM_LBUTTONDOWN:
            Console::Out("X: ", GET_X_LPARAM(lParam));
            Console::Out("Y: ", GET_Y_LPARAM(lParam));
        break;
        case WM_KEYDOWN:
            if(wParam == 'P'){
                pause = !pause;
                if(pause)
                    Console::Out("Paused");
                else
                    Console::Out("Resume");
            }
        break;
        default:
            return DefWindowProc(wnd, uMsg, wParam, lParam);
        break;
    }
    return 0;
}