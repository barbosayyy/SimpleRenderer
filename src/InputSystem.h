#pragma once
#include "IInputSystem.h"

class InputSystem : public IInputSystem {
public:
    InputSystem() {};
    ~InputSystem() {};

    void Init() override;
    void Shutdown() override;
    void ProcessInput();
    void RegisterAction(std::function<void()> func) override;
    bool GetKeyPressDown(int key) override;
private:
    std::vector<std::function<void()>> _onKeyboardFunctions;
};