#include "Renderer.h"
#include <iostream>
#include "glm/ext/matrix_transform.hpp"

//
// -- TODO: - Buffer utility functions (buffer swap, clear with bitmask, ..)
//          - 
//

void RendererSystem::Init(IWindowSystem* windowSystem, IObjectSystem* objectSystem){
    System::Init();
    for(int i = 0; i < 1024*768; i++){
        _pZBuffer[i] = std::numeric_limits<float>::infinity();
    }
    _windowSystem = windowSystem;
    _objectSystem = objectSystem;
}

void RendererSystem::Shutdown(){
    // Set frame to black
    memset(_pColorBuffer, 0, _windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height * 4);
    
    // Swap Buffer
    HDC hdcWin = GetDC(_windowSystem->GetMainWindow()->GetHandle());
    BitBlt(hdcWin, 0, 0, _windowSystem->GetMainWindow()->_width, _windowSystem->GetMainWindow()->_height, _deviceContext, 0, 0, SRCCOPY);;
    ReleaseDC(_windowSystem->GetMainWindow()->GetHandle(), hdcWin);

    System::Shutdown();
}

RendererSystem::RendererSystem() : _mainCamera(Camera(glm::vec3(0,0,5))) {

}

RendererSystem::~RendererSystem(){
    Shutdown();
    free(_pBackBuffer);
    free(_pColorBuffer);
    free(_pColorBuffer);
    DeleteObject(_frameBitmap);
    ReleaseDC(_windowSystem->GetMainWindow()->GetHandle(), _deviceContext);
}

void RendererSystem::PrepareDeviceContext()
{
    HDC dc = GetDC(_windowSystem->GetMainWindow()->GetHandle());
    _deviceContext = CreateCompatibleDC(dc);
    ReleaseDC(_windowSystem->GetMainWindow()->GetHandle(), dc);
    CreateBackbuffer();
    SelectObject(_deviceContext, _frameBitmap);
}

void RendererSystem::InitBuffers()
{
    memset(_pBackBuffer, 0, _windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height*4);
    std::cout << "Back ";

    _pColorBuffer = new unsigned long[_windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height * 4];
    for(int i = 0; i < _windowSystem->GetMainWindow()->_height;++i){
        for(int j = 0; j < _windowSystem->GetMainWindow()->_width; ++j){
            _pColorBuffer[i*_windowSystem->GetMainWindow()->_width+j] = Color(255,255,255,255);
        }
    }
    std::cout << "Color ";

    std::cout << std::endl;
}

void RendererSystem::Render(){
    if(IsActive()){
        memset(_pColorBuffer, 0, _windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height * 4);

        for(int i = 0; i < _windowSystem->GetMainWindow()->_height; ++i){
            for(int j = 0; j < _windowSystem->GetMainWindow()->_width; ++j){
                // Background color
                _pColorBuffer[i*_windowSystem->GetMainWindow()->_width+j] = Color(0x13,0x33,0x37,0xff);
                for(int f = 0; f < _objectSystem->GetObjectVector().size();f++){
                    if(_objectSystem->GetObjectVector().at(f).Rasterize(j, i, color))
                        _pColorBuffer[i*_windowSystem->GetMainWindow()->_width+j] = color;
                }
            }
        }

        memcpy(_pBackBuffer, _pColorBuffer, _windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height * 4);

        // Swap Buffer
        HDC hdcWin = GetDC(_windowSystem->GetMainWindow()->GetHandle());
        BitBlt(hdcWin, 0, 0, _windowSystem->GetMainWindow()->_width, _windowSystem->GetMainWindow()->_height, _deviceContext, 0, 0, SRCCOPY);;
        ReleaseDC(_windowSystem->GetMainWindow()->GetHandle(), hdcWin);
    }
}

void RendererSystem::DrawLine(int x0, int y0, int x1, int y1, unsigned long* buf, int col)
{
    for(float t=0.0; t<1.0;t+=0.01){
        int x = x0 + (x1-x0)*t;
        int y = y0 + (y1-y0)*t;
        if(GetIndexInMatrix(1024, 768, x, y) > -1)
            buf[GetIndexInMatrix(1024, 768, x, y)] = col;
    }
}

void RendererSystem::DrawPoint(int x, int y, unsigned long* buf, int col)
{
    if(GetIndexInMatrix(1024, 768, x, y) > -1)
        buf[GetIndexInMatrix(1024, 768, x, y)] = col;
}

int Color(int red, int green, int blue, int alpha){
    if(red > 255 || red < 0){
        red = 255;
    }
    if(green > 255 || green < 0){
        green = 255;
    }
    if(blue > 255 || blue < 0){
        blue = 255;
    }

    return (alpha << 24) | (red << 16) | (green << 8) | blue << 0;
}

void RendererSystem::CreateBackbuffer() {
    _frameBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    _frameBitmapInfo.bmiHeader.biWidth = _windowSystem->GetMainWindow()->_width;
    _frameBitmapInfo.bmiHeader.biHeight = -_windowSystem->GetMainWindow()->_height;
    _frameBitmapInfo.bmiHeader.biPlanes = 1;
    _frameBitmapInfo.bmiHeader.biBitCount = 32;
    _frameBitmapInfo.bmiHeader.biCompression = BI_RGB;
    _frameBitmapInfo.bmiHeader.biSizeImage = 0;
    _frameBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
    _frameBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
    _frameBitmapInfo.bmiHeader.biClrUsed = 0;
    _frameBitmapInfo.bmiHeader.biClrImportant = 0;
    if(_frameBitmap)
        DeleteObject(_frameBitmap);
    _frameBitmap = CreateDIBSection(_deviceContext, &_frameBitmapInfo, DIB_RGB_COLORS, &_pBackBuffer, NULL, 0);
    if(!_frameBitmap)
        DeleteObject(_frameBitmap);
}

void RendererSystem::UpdateBackbuffer() {
    _frameBitmapInfo.bmiHeader.biWidth = _windowSystem->GetMainWindow()->_width;
    _frameBitmapInfo.bmiHeader.biHeight = -_windowSystem->GetMainWindow()->_height;
    if(_frameBitmap){
        DeleteObject(_frameBitmap);
    }
    _frameBitmap = CreateDIBSection(_deviceContext, &_frameBitmapInfo, DIB_RGB_COLORS, (void**)&_pBackBuffer, NULL, 0);
    if(!_frameBitmap){
        DeleteObject(_frameBitmap);
    }
}

void RendererSystem::ResizeWindow(){
    if(_windowSystem->GetMainWindow()->_newHeight != _windowSystem->GetMainWindow()->_height || _windowSystem->GetMainWindow()->_newWidth != _windowSystem->GetMainWindow()->_width){   
        _windowSystem->GetMainWindow()->_width = _windowSystem->GetMainWindow()->_newWidth;
        _windowSystem->GetMainWindow()->_height = _windowSystem->GetMainWindow()->_newHeight;
        UpdateBackbuffer();
        SelectObject(_deviceContext, _frameBitmap);
    }
}