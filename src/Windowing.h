#pragma once

#include "pch.h"
#include "System.h"
#include <vector>

const char windowClassName[] = "Simple Renderer";
const char windowName[] = "Renderer";

LRESULT CALLBACK WndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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

class IWindowSystem : public System{
public:
    virtual Window* GetMainWindow() = 0;
    virtual Window* GetWindowByHandle(HWND handle) = 0;
    virtual void AddWindow(Window* window) = 0;
    virtual bool MainWindowNotClosed() = 0;
    virtual void SetInstance(HINSTANCE hInstance) = 0;
    virtual ~IWindowSystem() = default;
};

class WindowSystem : public IWindowSystem{
    public:
        void Init() override;
        void Shutdown() override;
        std::vector<Window*> _windows;
        WindowSystem();
        ~WindowSystem() {Shutdown();};

        Window* GetMainWindow() override {if(MainWindowNotClosed()){return _mainWindow;}else{return nullptr;};};
        Window* GetWindowByHandle(HWND handle) override;
        void AddWindow(Window* window) override;
        bool MainWindowNotClosed() override {if(_mainWindow->GetHandle() != nullptr){return true;}else{return false;}};
        void SetInstance(HINSTANCE hInstance) override {_hInstance = hInstance;};
    private:
        Window* _mainWindow;
        HINSTANCE _hInstance {nullptr};
        WNDPROC _wndProc {nullptr};
        unsigned int selectedWindow {0};
};

