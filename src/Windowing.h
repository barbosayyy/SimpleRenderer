#pragma once
#include "pch.h"
#include "System.h"
#include <vector>

const char windowClassName[] = "Simple Renderer";
const char windowName[] = "Renderer";

// // Window message procedure
// LRESULT CALLBACK WndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window{
    public:
        HWND RegisterWindowClass(HINSTANCE hInstance);
        int _width {0};
        int _height {0};
        int _newWidth {0};
        int _newHeight {0};    
        HWND GetHandle() {return _handle;};
        Window(HINSTANCE hInstance);
        Window(HINSTANCE hInstance, int width, int height);
        void Show(int nCmdShow) {ShowWindow(_handle, nCmdShow);};
    private:
        HWND _handle;
};

class WindowSystem : public System{
    public:
        std::vector<Window*> _windows;
        WindowSystem();
        Window* GetMainWindow() {if(MainWindowNotClosed()){return _mainWindow;}else{return nullptr;};};
        Window* GetWindowByHandle(HWND handle);
        void AddWindow(Window* window);
        bool MainWindowNotClosed() {if(_mainWindow->GetHandle() != nullptr){return true;}else{return false;}};
        void SetInstance(HINSTANCE hInstance) {_hInstance = hInstance;};
        void Init() override;
        void Shutdown() override;
    private:
        Window* _mainWindow;
        HINSTANCE _hInstance {nullptr};
        WNDPROC _wndProc {nullptr};
        unsigned int selectedWindow {0};
};

