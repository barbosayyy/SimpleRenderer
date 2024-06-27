#pragma once
#include "Core.h"

int GetIndexInMatrix(int width, int height, int x, int y);
float ProjRemap(float value);

inline float EdgeInverted(Vector2 p, Vector2 p0, Vector2 p1){
    return (p0.x-p1.x)*(p.y-p0.y)-(p0.y-p1.y)*(p.x-p0.x);
}

inline float Barycentric(Vector2 p, Vector2 v0, Vector2 v1, Vector2 v2){
    return (EdgeInverted(p, v1, v2))/EdgeInverted(v0, v1, v2);
};