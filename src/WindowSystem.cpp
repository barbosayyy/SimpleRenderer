#include "WindowSystem.h"
#include "Console.h"
#include "RendererSystem.h"

// Window System

WindowSystem::WindowSystem() : _windows(std::vector<Window*>{}), _mainWindow(nullptr){

}

Window* WindowSystem::GetMainWindow(){
    if(IsActive()){
        if(_mainWindow != nullptr){
            return _mainWindow;
        }
    }
    return nullptr;
}

void WindowSystem::Init(HINSTANCE hInstance){
    System::Init();
    if(IsActive() == true){
        _hInstance = hInstance;
        Window* window = new Window(_hInstance);
        AddWindow(window);
        // Out Info
            Console::Out("Window Handle address: ", GetMainWindow()->GetHandle());
            Console::Out("Width: ", cfgDefaultWindowWidth);
            Console::Out("Height: ", cfgDefaultWindowHeight);
            Console::Out("Window Init");
    }
}

void WindowSystem::Shutdown() {
    if(IsActive()){
        for(int i = _windows.size()-1; i == 0; i--){
            if(_windows.at(i) != nullptr){
                delete _windows.at(i);
                _windows.at(i) = nullptr;
                _windows.pop_back();
            }
        }
        _mainWindow = nullptr;
        System::Shutdown();
    }
}

void WindowSystem::AddWindow(Window* window){
    if(IsActive() == true){
        if(GetWindowByHandle(window->GetHandle()) == nullptr){
            if(_windows.empty()){
                _mainWindow = window;
            }
            _windows.push_back(window);
        }
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