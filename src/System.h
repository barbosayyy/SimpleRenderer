#pragma once
#include "Core.h"

enum class SystemState{
    DISABLED = 0,
    ACTIVE = 1
};

class System{
public:
    virtual void Init(){
        _state = SystemState::ACTIVE;
    };
    virtual void Shutdown(){
        _state = SystemState::DISABLED;
    };
    virtual bool IsActive(){
        if(_state == SystemState::ACTIVE){
            return true;
        }
        else{
            return false;
        }
    }; 
    virtual ~System() = default;
protected:
    SystemState _state;
    System() : _state(SystemState::DISABLED){};
};

