#pragma once

#include "pch.h"
#include "Core.h"
#include "Math.h"
#include "Windowing.h"
#include "Object.h"
#include "System.h"
#include "Camera.h"

int Color(int red, int green, int blue, int alpha);
void InitColorBuffer(unsigned long*& buf, int width, int height);

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

class RendererSystem : public IRendererSystem{
    public:
        void Init(IWindowSystem* windowSystem, IObjectSystem* objectSystem);
        void Shutdown() override;
        
        RendererSystem();
        ~RendererSystem();
        void Render() override;
        void DrawLine(int x0, int y0, int x1, int y1, unsigned long* buf, int col) override;
        void DrawPoint(int x, int y, unsigned long* buf, int col) override;
        void CreateBackbuffer() override;
        void UpdateBackbuffer() override;
        void ResizeWindow() override;
        Camera& GetMainCamera() override {return _mainCamera;};

        IWindowSystem* GetWindowSystem() {return _windowSystem;};
        void SetMainCamera(Camera camera) {_mainCamera = camera;};

        void PrepareDeviceContext();
        void InitBuffers();
        // void ClearWindowSystem()
    private:
        IWindowSystem* _windowSystem;
        IObjectSystem* _objectSystem;
        BITMAPINFO _frameBitmapInfo = {0};
        HBITMAP _frameBitmap = 0;
        HDC _deviceContext = 0;
        Camera _mainCamera;

        void* _pBackBuffer;
        unsigned long* _pColorBuffer;
        float* _pZBuffer = new float[1024*768];

        int color{0x00000000};
};