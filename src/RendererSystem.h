#pragma once

#include "pch.h"
#include "Math.h"
#include "IRendererSystem.h"
#include "IWindowSystem.h"
#include <vector>
#include "RenderThreadPool.h"

int color(int red, int green, int blue, int alpha);

class RendererSystem : public IRendererSystem{
    public:
        RendererSystem();
        ~RendererSystem(){};
    
        void Init(IWindowSystem* windowSystem);
        void Shutdown();
        void Render() override;
        void DrawLine(int x0, int y0, int x1, int y1, unsigned long* buf, int col) override;
        void DrawPoint(int x, int y, unsigned long* buf, int col) override;
        void ResizeWindow() override;
        Camera& GetMainCamera() override {return _mainCamera;};
        void AddTriangle(Triangle triangle) override;

        IWindowSystem* GetWindowSystem() {return _windowSystem;};
        void SetMainCamera(Camera camera) {_mainCamera = camera;};
        std::vector<Triangle>& GetTriangles() {return _triangles;};
        void PollRecompute();
        void RenderTask(int tilex0, int tiley0, int tilex1, int tiley1);

        void PrepareDeviceContext();
        void InitFrameBackbuffer();
        void InitFramebuffers();
        void UpdateFramebuffers();
        void SwapFramebuffers();
        void ClearFramebuffers();

        int& GetCurrentFrame() {return _currentFrame;};
        
        bool IsRenderingEnabled() {return _isRenderingEnabled;};
        void ToggleRendering(bool toggle);
        void ReleaseContext();

    private:
        bool _isMultithreaded {false};
        bool _smallTiles {false}; // Must remain off for now
        int _currentFrame;
        bool _isRenderingEnabled {false};

        IWindowSystem* _windowSystem;
        BITMAPINFO _frameBitmapInfo = {0};
        HBITMAP _frameBitmap = 0;
        HDC _deviceContext = 0;
        Camera _mainCamera;
        RenderThreadPool _renderThreadPool;

        int _bufferWidth {0};
        int _bufferHeight {0};

        // GDI frame buffer
        void* _pBackBuffer;

        // Color frame buffer
        unsigned long* _pColorBuffer;

        // Depth frame buffer
        float* _pZBuffer;
        std::vector<Triangle> _triangles;
        int _color{0x00000000};
};