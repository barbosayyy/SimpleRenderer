#pragma once
#include "IWindowSystem.h"
#include <vector>

class WindowSystem : public IWindowSystem{
    public:
        void Init(HINSTANCE hInstance);
        void Shutdown() override;
        std::vector<Window*> _windows;
        WindowSystem();
        ~WindowSystem() {Shutdown();};

        Window* GetMainWindow() override {if(MainWindowNotClosed()){return _mainWindow;}else{return nullptr;};};
        Window* GetWindowByHandle(HWND handle) override;
        void AddWindow(Window* window) override;
        bool MainWindowNotClosed() override {if(_mainWindow->GetHandle() != nullptr){return true;}else{return false;}};
    private:
        Window* _mainWindow;
        HINSTANCE _hInstance {nullptr};
        WNDPROC _wndProc {nullptr};
        unsigned int selectedWindow {0};
};

