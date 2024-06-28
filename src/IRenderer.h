#pragma once

#include "Core.h"
#include "System.h"
#include "Camera.h"

class IRendererSystem : public System{
public:
    virtual void Render() = 0;
    virtual void DrawLine(int x0, int y0, int x1, int y1, unsigned long* buf, int col) = 0;
    virtual void DrawPoint(int x, int y, unsigned long* buf, int col) = 0;
    virtual void CreateBackbuffer() = 0;
    virtual void UpdateBackbuffer() = 0;
    virtual void ResizeWindow() = 0;
    virtual Camera& GetMainCamera() = 0;
    virtual ~IRendererSystem() = default;
};