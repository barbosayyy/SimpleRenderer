#pragma once
#include "System.h"
#include "Window.h"

class IWindowSystem : public System{
public:
    virtual Window* GetMainWindow() = 0;
    virtual Window* GetWindowByHandle(HWND handle) = 0;
    virtual void AddWindow(Window* window) = 0;
    virtual void MoveWindow(int x, int y) = 0;
    virtual ~IWindowSystem() = default;
};