#pragma once

#include <vector>

constexpr unsigned int ARGB_RED = 0x00FF0000;
constexpr unsigned int ARGB_GREEN = 0x0000FF00;
constexpr unsigned int ARGB_BLUE = 0x000000FF;
constexpr unsigned int ARGB_DARKTEAL = 0x00133337;

typedef unsigned char RGB[3];

constexpr int cfgDefaultWindowWidth = 1024;
constexpr int cfgDefaultWindowHeight = 768;

struct Vector2{
    float x;
    float y;
    Vector2(float x, float y) : x(x), y(y){};
};

struct Vector3{
    float x;
    float y;
    float z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {};
};