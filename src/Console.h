#pragma once
#include <iostream>

namespace Console{
    void Create();
    template <typename ...Args>
    void Out(Args... args){
        (std::cout << ... << args) << std::endl;
    }
    template <typename ...Args>
    void OutInline(Args... args){
        (std::cout << ... << args);
    }
}