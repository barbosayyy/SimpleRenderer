#pragma once

#include "Core.h"
#include "Primitives.h"
#include "System.h"

#include "IRenderer.h"

class IObjectSystem : public System{
public:
    virtual ~IObjectSystem() = default;
    virtual std::vector<Triangle>& GetObjectVector() = 0;
};

class ObjectSystem : public IObjectSystem{
public:
    void Init() override;
    void Shutdown() override;

    std::vector<Triangle>& GetObjectVector() override {return _objects;};
    
    ObjectSystem();
    ~ObjectSystem(){Shutdown();};
    void AddTriangle(Triangle drawable);
private:
    std::vector<Triangle> _objects;
};