#pragma once
#include "Core.h"

class System{
    public:
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
        System() {};
};