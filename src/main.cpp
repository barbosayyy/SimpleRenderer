#include "pch.h"

#include "Windowing.h"

#include <iostream>
#include <vector>
#include <algorithm>

WindowSystem windowSystem;

//
// -- TODO: - Rasterization algorithm       
//          - Z-buffer and depth check (near, far)
//          - 
//

BITMAPINFO frameBitmapInfo = {0};
HBITMAP frameBitmap = 0;
HDC deviceContext = 0;
void* pFrameBits;
float* zBuffer = new float[1024*720];

void CreateConsole() {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
    std::ios::sync_with_stdio();
}

void UpdateBackbuffer(BITMAPINFO& bitmapInfo, HBITMAP& bitmap, void*& pBits, Window w) {
    bitmapInfo.bmiHeader.biWidth = w._width;
    bitmapInfo.bmiHeader.biHeight = -w._height;
    if(bitmap){
        DeleteObject(bitmap);
    }
    bitmap = CreateDIBSection(deviceContext, &bitmapInfo, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
    if(!bitmap){
        DeleteObject(bitmap);
    }
}

void CreateBackbuffer(BITMAPINFO& bitmapInfo, HBITMAP& bitmap, void*& pBits, Window w) {
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = w._width;
    bitmapInfo.bmiHeader.biHeight = -w._height;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;
    bitmapInfo.bmiHeader.biSizeImage = 0;
    bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
    bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
    bitmapInfo.bmiHeader.biClrUsed = 0;
    bitmapInfo.bmiHeader.biClrImportant = 0;
    if(bitmap)
        DeleteObject(bitmap);
    bitmap = CreateDIBSection(deviceContext, &bitmapInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
    if(!bitmap)
        DeleteObject(bitmap);
}

void ResizeWindow(BITMAPINFO& bitmapInfo, HBITMAP& bitmap, void*& pBits, Window* w){
    if(w->_newHeight != w->_height || w->_newWidth != w->_width){   
        w->_width = w->_newWidth;
        w->_height = w->_newHeight;
        UpdateBackbuffer(bitmapInfo, bitmap, pBits, *w);
        SelectObject(deviceContext, bitmap);
    }
}

class Triangle{
    public:
        Vector2 corners[3] = {
        { -1, 1},
            { -1, -1},
            { 2,  -1},
        };
        Vector2 projectedCorners[3] = {
            {0,0},
            {0,0},
            {0,0}
        };
        Triangle();
        bool RenderPixel(int x, int y);
        void PrintProjCorners();
        int color = 0x5D5D5D;
        int minX, minY, maxX, maxY;
};

void Triangle::PrintProjCorners(){
    for(int i = 0; i< 3; i++){
        std::cout << projectedCorners[i].x << " " << projectedCorners[i].y << std::endl;
    }
    std::cout << minX << " " << minY << " " << maxX << " " << maxY << std::endl;
}

Triangle::Triangle() : minX(-1), minY(-1), maxX(-1), maxY(-1){
    int tminX {0}, tminY {0}, tmaxX {0}, tmaxY {0};
    for(int i=0;i<3;i++){
        float x_proj = corners[i].x;
        float y_proj = corners[i].y;
        float x_proj_remap = (1+x_proj)/2;
        float y_proj_remap = (1+y_proj)/2;
        projectedCorners[i].x = x_proj_remap * cfgDefaultWindowWidth;
        projectedCorners[i].y = y_proj_remap * cfgDefaultWindowHeight;

        if(tminX == -1 || projectedCorners[i].x < tminX){
            tminX = projectedCorners[i].x;
        }
        if(tminY == -1 || projectedCorners[i].y < tminY){
            tminY = projectedCorners[i].y;
        }
        if(tmaxX == -1 || projectedCorners[i].x > tmaxX){
            tmaxX = projectedCorners[i].x;
        }
        if(tmaxY == -1 || projectedCorners[i].y > tmaxY){
            tmaxY = projectedCorners[i].y;
        }
        minX = std::max(0, std::min(cfgDefaultWindowWidth-1, tminX));
        minY = std::max(0, std::min(cfgDefaultWindowHeight-1, tminY));
        maxX = std::max(0, std::min(cfgDefaultWindowWidth-1, tmaxX));
        maxY = std::max(0, std::min(cfgDefaultWindowHeight-1, tmaxY));
    }
    PrintProjCorners();
}

bool Triangle::RenderPixel(int x, int y){
    if(x > minX && x < maxX && y > minY && y < maxY){
        return true;
    }
    return false;
}

class Cube{
    public:
        Vector3 corners[8] = {
        { 1, -1, -5},
            { 1, -1, -3},
            { 1,  1, -5},
            { 1,  1, -3},
            {-1, -1, -5},
            {-1, -1, -3},
            {-1,  1, -5},
            {-1,  1, -3}
        };
        Vector2 projectedCorners[8] = {
            {0,0},
            {0,0},
            {0,0},
            {0,0},
            {0,0},
            {0,0},
            {0,0},
            {0,0}
        };
        Cube();
        bool RenderPixel(int x, int y);
        void PrintProjCorners();
};

void Cube::PrintProjCorners(){
    for(int i = 0; i< 8; i++){
        std::cout << projectedCorners[i].x << " " << projectedCorners[i].y << std::endl;
    }
}

Cube::Cube(){
    for(int i=0;i<8;i++){
        float x_proj = corners[i].x / -corners[i].z;
        float y_proj = corners[i].y / -corners[i].z;
        float x_proj_remap = (1+x_proj)/2;
        float y_proj_remap = (1+y_proj)/2;
        projectedCorners[i].x = x_proj_remap * cfgDefaultWindowWidth;
        projectedCorners[i].y = y_proj_remap * cfgDefaultWindowHeight;
    }
    PrintProjCorners();
}

bool Cube::RenderPixel(int x, int y){
    
}

std::vector<Triangle> triangles {};

void RenderInit(){
    for(int i = 0; i < 1024*720; i++){
        zBuffer[i] = std::numeric_limits<float>::infinity();
    }
}

void RenderMain(void* pBuffer, Window w, int col1, int col2){
    memset(pBuffer, 0, w._width * w._height * 4);
    DWORD* pixel = (DWORD*)pBuffer;
    int k{0};
    for(int i = 0; i < w._height; ++i){
        for(int j = 0; j < w._width; ++j){
            for(int f = 0; f < triangles.size();f++){
                if(triangles.at(0).RenderPixel(i, j) == true){
                    pixel[k] = 0xADADAD;
                }
                else{
                    pixel[k] = 0x3d3d3d;
                }
            }
            k++;
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    CreateConsole();

    windowSystem.SetInstance(hInstance);
    windowSystem.Init();
    windowSystem.GetMainWindow()->Show(nCmdShow);
    
    HDC dc = GetDC(windowSystem.GetMainWindow()->GetHandle());
    deviceContext = CreateCompatibleDC(dc);
    ReleaseDC(windowSystem.GetMainWindow()->GetHandle(), dc);
    CreateBackbuffer(frameBitmapInfo, frameBitmap, pFrameBits, *windowSystem.GetMainWindow());
    SelectObject(deviceContext, frameBitmap);

    memset(pFrameBits, 0, windowSystem.GetMainWindow()->_width * windowSystem.GetMainWindow()->_height*4);

    // Cube cube;
    Triangle triangle;
    triangles.push_back(triangle);

    MSG msg = {};
    while(msg.message != WM_QUIT && windowSystem.MainWindowNotClosed() == true){
        
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else{
            std::cout << windowSystem.GetMainWindow()->_newWidth << " " << windowSystem.GetMainWindow()->_height << std::endl;
            // Render
            ResizeWindow(frameBitmapInfo, frameBitmap, pFrameBits, windowSystem.GetMainWindow());

            RenderMain(pFrameBits, *windowSystem.GetMainWindow(), 0x1D1D1D, 0x3D3D3D);

            // Copy buffer to window dc
            HDC hdcWin = GetDC(windowSystem.GetMainWindow()->GetHandle());
            BitBlt(hdcWin, 0, 0, windowSystem.GetMainWindow()->_width, windowSystem.GetMainWindow()->_height, deviceContext, 0, 0, SRCCOPY);
            ReleaseDC(windowSystem.GetMainWindow()->GetHandle(), hdcWin);
        }
    }
    DeleteObject(frameBitmap);
    ReleaseDC(windowSystem.GetMainWindow()->GetHandle(), deviceContext);
    return (int)msg.wParam;
}