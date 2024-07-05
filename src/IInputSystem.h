#pragma once
#include "System.h"
#include <functional>

class IInputSystem : public System {
public:
    virtual void RegisterAction(std::function<void()> func) = 0;
    virtual bool GetKeyPressDown(int key) = 0;
    virtual ~IInputSystem() = default;
};