#pragma once

#include "Core.h"
#include "glm/glm.hpp"

class Drawable{
public:
    virtual bool Rasterize(int x, int y, int& color) = 0;
protected:
    Drawable(){};
};

class Triangle : public Drawable{
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

    bool Draw(int x, int y, int& color);

    void ComputeVertices();
    
    bool Rasterize(int x, int y, int& color) override;
    
    void PrintProjectedCorners();
    
    glm::vec4 _projected[3];
private:
    int _minX, _minY, _maxX, _maxY;
};