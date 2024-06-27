#include "Windowing.h"
#include <iostream>

// Window

Window::Window(HINSTANCE hInstance) : 
    _width(cfgDefaultWindowWidth),
    _height(cfgDefaultWindowHeight),
    _newWidth(cfgDefaultWindowWidth),
    _newHeight(cfgDefaultWindowHeight){

        RegisterWindowClass(hInstance);
}

Window::Window(HINSTANCE hInstance, int width, int height) : 
    _width(width),
    _height(height),
    _newWidth(width),
    _newHeight(height){

        RegisterWindowClass(hInstance);
}

HWND Window::RegisterWindowClass(HINSTANCE hInstance){
    WNDCLASS windowClass = {};
    windowClass.lpfnWndProc = WndProc;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = windowClassName;
    RegisterClass(&windowClass);
    RECT rect = {0, 0, _width, _height};
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, true);
    HWND window = CreateWindowEx(
            0,
            windowClassName,    
        windowName,
        WS_OVERLAPPEDWINDOW,   
        CW_USEDEFAULT, CW_USEDEFAULT,rect.right-rect.left, rect.bottom-rect.top,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    _handle = window;
    return window;
}

// Window System

WindowSystem::WindowSystem() : _windows(std::vector<Window*>{}), _mainWindow(nullptr){

}

void WindowSystem::Init() {
    System::Init();
    Window* window = new Window(_hInstance);
    AddWindow(window);
    std::cout << "Window Handle address: " << GetMainWindow()->GetHandle() << std::endl;
}

void WindowSystem::Shutdown() {
    for(int i=0;_windows.size() == 0;i++){
        _windows.pop_back();
    }
    System::Shutdown();
}

void WindowSystem::AddWindow(Window* window){
    if(GetWindowByHandle(window->GetHandle()) == nullptr){
        if(_windows.size() == 0){
            _mainWindow = window;
        }
        _windows.push_back(window);
    }
}

Window* WindowSystem::GetWindowByHandle(HWND handle){
    for(int i = 0; i<_windows.size();i++){
        if(_windows.at(i)->GetHandle() == handle){
            return _windows.at(i);
        }
    }
    return nullptr;
}

// Window Message Procedure

extern WindowSystem windowSystem;

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
                    std::cout << windowSystem.GetWindowByHandle(wnd)->_newWidth << std::endl;
                    std::cout << windowSystem.GetWindowByHandle(wnd)->_newHeight << std::endl;
                }
            }
        break;
        case WM_KEYDOWN:
        break;
        default:
            return DefWindowProc(wnd, uMsg, wParam, lParam);
        break;
    }
    return 0;
}