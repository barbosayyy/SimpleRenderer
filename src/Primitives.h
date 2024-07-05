#pragma once

#include "Core.h"
#include "glm/glm.hpp"

class Triangle{
public:
    Vector3 _v[3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
    };
    Vector3 _pV[3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    Triangle(Vector3 v0, Vector3 v1, Vector3 v2);

    void ComputeVertices();

    bool BoundingBox(int x, int y);

    bool Barycentric(int pX, int pY, float &w0, float &w1, float &w2);

    void Rasterize(float w0, float w1, float w2, int& color);
    
    void PrintProjectedCorners();
    
    glm::vec4 _projected[3];
private:
    int _minX, _minY, _maxX, _maxY;
};