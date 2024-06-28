#pragma once

#include "pch.h"
#include "Math.h"
#include "IRenderer.h"
#include "Windowing.h"
#include "Object.h"

int Color(int red, int green, int blue, int alpha);
void InitColorBuffer(unsigned long*& buf, int width, int height);

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
        std::vector<glm::mat4> _viewMatrices;

        int color{0x00000000};
};