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

LRESULT CALLBACK WndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg){
        case WM_DESTROY:
            if(windowSystem.MainWindowNotClosed() == false)
                PostQuitMessage(0);
            return 0;
        break;
        case WM_SIZE: {
                if(windowSystem.GetWindowByHandle(wnd) != nullptr){
                    RECT rect;
                    GetClientRect(wnd, &rect);
                    windowSystem.GetWindowByHandle(wnd)->_newWidth = rect.right - rect.left;
                    windowSystem.GetWindowByHandle(wnd)->_newHeight = rect.bottom - rect.top;
                    Console::Out(windowSystem.GetWindowByHandle(wnd)->_newWidth);
                    Console::Out(windowSystem.GetWindowByHandle(wnd)->_newHeight);
                }
            }
        break;
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