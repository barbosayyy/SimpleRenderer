#include "pch.h"
#include "Console.h"
#include <iostream>

void Console::Create() {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
    std::ios::sync_with_stdio();
}

// template <typename ...Args>
// void Console::OutL(Args... args)

// template <typename ...Args>
// void Console::Out(Args... args)