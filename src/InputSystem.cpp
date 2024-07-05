#include "InputSystem.h"
#include "Console.h"
#include "pch.h"
#include <winuser.h>

void InputSystem::Init()
{
    System::Init();
    
    Console::Out("Input Init");
}

void InputSystem::Shutdown()
{
    
}

bool InputSystem::GetKeyPressDown(int key)
{
    return GetAsyncKeyState(key);
}

void InputSystem::ProcessInput()
{
    for(std::function<void()> func : _onKeyboardFunctions){
        func();
    }
}

void InputSystem::RegisterAction(std::function<void()> func)
{
    _onKeyboardFunctions.push_back(func);
}
