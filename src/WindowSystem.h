#pragma once
#include "IWindowSystem.h"
#include <vector>

class WindowSystem : public IWindowSystem{
    public:
        void Init(HINSTANCE hInstance);
        void Shutdown();
        std::vector<Window*> _windows;
        WindowSystem();
        ~WindowSystem(){};

        Window* GetMainWindow() override;
        Window* GetWindowByHandle(HWND handle) override;
        void AddWindow(Window* window) override;
        void MoveWindow(int x, int y) override {_mainWindow->Move((x), (y));};
    private:
        Window* _mainWindow;
        HINSTANCE _hInstance {nullptr};
        WNDPROC _wndProc {nullptr};
        unsigned int selectedWindow {0};
};

