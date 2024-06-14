#pragma once



enum class ARGBColor{
    RED = 0xFF0000,
    GREEN = 0x00FF00,
    BLUE = 0x0000FF,
    DARKGREEN = 0x133337
};

constexpr int cfgDefaultWindowWidth = 1024;
constexpr int cfgDefaultWindowHeight = 720;

struct Vector2{
    float x;
    float y;
};

struct Vector3{
    float x;
    float y;
    float z;
};

int GetMatPos(int width, int x, int y);
float ProjRemap(float value);
