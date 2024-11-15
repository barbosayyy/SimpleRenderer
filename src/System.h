#pragma once
#include "Core.h"

enum class SystemState{
    DISABLED = 0,
    ACTIVE = 1
};

class System{
public:
    void Init(){
        _state = SystemState::ACTIVE;
    };
    void Shutdown(){
        _state = SystemState::DISABLED;
    };
    bool IsActive(){
        return _state == SystemState::ACTIVE;
    }; 
    ~System() = default;
protected:
    SystemState _state;
    System() : _state(SystemState::DISABLED){};
};

