#include "WindowSystem.h"
#include "Console.h"
#include "RendererSystem.h"

// Window System

WindowSystem::WindowSystem() : _windows(std::vector<Window*>{}), _mainWindow(nullptr){

}

void WindowSystem::Init(HINSTANCE hInstance) {
    System::Init();
    _hInstance = hInstance;
    Window* window = new Window(_hInstance);
    AddWindow(window);
    // Out Info
        Console::Out("Window Handle address: ", GetMainWindow()->GetHandle());
        Console::Out("Width: ", cfgDefaultWindowWidth);
        Console::Out("Height: ", cfgDefaultWindowHeight);
        Console::Out("Window Init");
}

void WindowSystem::Shutdown() {
    for(int i=0;_windows.size() == 0;i++){
        delete _windows.at(i);
        _windows.pop_back();
    }
    delete _mainWindow;
    _mainWindow = nullptr;
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