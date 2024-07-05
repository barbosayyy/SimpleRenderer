#include "RendererSystem.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "Console.h"

//
// -- TODO: - Buffer utility functions (buffer swap, clear with bitmask, ..)
//          - 
//

bool renderShapes = 1;

int color(int red, int green, int blue, int alpha){
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

// _renderThreadPool starts with possible max amount of threads
RendererSystem::RendererSystem() : _mainCamera(Camera(glm::vec3(0,0,5))), _renderThreadPool(std::thread::hardware_concurrency()) {    

}

RendererSystem::~RendererSystem(){
    Shutdown();
}

void RendererSystem::Init(IWindowSystem* windowSystem){
    System::Init();
    _windowSystem = windowSystem;
    Console::Out("Renderer Init");
    
    Console::Out("Renderer - Assigning device context");
    this->PrepareDeviceContext();
    Console::Out("Renderer - Initializing buffers: ");
    this->InitBuffers();

    _multiThreaded = true;
    //Temp
    _smallTiles = false;
    _renderThreadPool.Start();
    Console::Out("Renderer - Started rendering thread pool ");
}

void RendererSystem::Shutdown(){
    // Set frame to black
    memset(_pColorBuffer, 0, _windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height * 4);
    memcpy(_pBackBuffer, _pColorBuffer, _windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height * 4);
    // Swap Buffer
    HDC hdcWin = GetDC(_windowSystem->GetMainWindow()->GetHandle());
    BitBlt(hdcWin, 0, 0, _windowSystem->GetMainWindow()->_width, _windowSystem->GetMainWindow()->_height, _deviceContext, 0, 0, SRCCOPY);;
    ReleaseDC(_windowSystem->GetMainWindow()->GetHandle(), hdcWin);
    delete[] _pColorBuffer;
    delete[] _pZBuffer;
    delete _windowSystem;
    DeleteObject(_frameBitmap);
    ReleaseDC(_windowSystem->GetMainWindow()->GetHandle(), _deviceContext);
    System::Shutdown();
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
    _pColorBuffer = new unsigned long[_windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height * 4];
    for(int i = 0; i < _windowSystem->GetMainWindow()->_height;++i){
        for(int j = 0; j < _windowSystem->GetMainWindow()->_width; ++j){
            _pColorBuffer[i*_windowSystem->GetMainWindow()->_width+j] = color(255,255,255,255);
        }
    }
    _pZBuffer = new float[_windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height];
    Console::OutInline("Color ");
    for(int i = 0; i < _windowSystem->GetMainWindow()->_width*_windowSystem->GetMainWindow()->_height; i++){
        _pZBuffer[i] = std::numeric_limits<float>::infinity();
    }
    Console::OutInline("Depth ");
    Console::Out();
}

void RendererSystem::Render(){
    if(IsActive()){
        ResizeWindow();

        ClearBuffers();

        PollRecompute();

        if(_multiThreaded){
            int x0, y0, x1, y1;
            int wWidth = _windowSystem->GetMainWindow()->_width;
            int wHeight = _windowSystem->GetMainWindow()->_height;
            int numWorkers = _renderThreadPool.GetNumWorkers();
            int hTiles;
            int vTiles;
            int xTileAmount;
            int yTileAmount;

            // Tile partition
            if(!_smallTiles){
                if(numWorkers >= 4 && numWorkers%2 == 0){
                    xTileAmount = numWorkers/2;
                    yTileAmount = 2;

                    for(int i = 0; i < numWorkers; i++){
                        if(i < numWorkers/2){
                            x0 = (wWidth/xTileAmount)*(i%xTileAmount);
                            y0 = 0;
                            x1 = x0 + (wWidth/xTileAmount);
                            y1 = wHeight/2;
                        }
                        else{
                            x0 = (wWidth / xTileAmount) * ((i - numWorkers / 2) % xTileAmount);
                            y0 = wHeight / 2;
                            x1 = x0 + (wWidth / xTileAmount);
                            y1 = wHeight;
                        }
                        if(i != 0){
                            x0++;
                            y0 = (y0 != 0) ? y0+1 : y0;
                        }
                        _renderThreadPool.EnqueueTask(std::bind(&RendererSystem::RenderTask, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
                        x0, y0, x1, y1);
                    }
                }
                else if(numWorkers >= 1){
                    xTileAmount = numWorkers;
                    yTileAmount = 1;
                    for(int i = 0; i < numWorkers; i++){
                        x0 = (wWidth/xTileAmount)*(i%xTileAmount);
                        y0 = 0;
                        x1 = x0 + (wWidth/xTileAmount);
                        y1 = wHeight;
                        if(numWorkers > 1 && i > 0){
                            x0++;
                        }
                        _renderThreadPool.EnqueueTask(std::bind(&RendererSystem::RenderTask, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
                        x0, y0, x1, y1);
                    }
                }
            }
            _renderThreadPool.WaitFrameComplete();
            SwapBuffers();
        }
        else{
            int currPx {0};
            int hPx {0};
            int wPx {0};
            float dist {0};
            float w0, w1, w2 {0};
            // frame pass 
            while(currPx < _windowSystem->GetMainWindow()->_width*_windowSystem->GetMainWindow()->_height-1){
                _pColorBuffer[currPx] = color(0x13,0x33,0x37,0xff);
                if(renderShapes){
                    for(int f = 0; f < _triangles.size();f++){
                        if(_triangles.at(f).BoundingBox(wPx, hPx)){
                            if(_triangles.at(f).Barycentric(wPx, hPx, w0, w1, w2)){
                                dist = glm::distance((w0*_triangles.at(f)._v[0].z+w1*_triangles.at(f)._v[1].z+w2*_triangles.at(f)._v[2].z), _mainCamera._wPos.z);
                                if(dist < _pZBuffer[currPx]){
                                    _pZBuffer[currPx] = dist;
                                    _triangles.at(f).Rasterize(w0, w1, w2, _color);
                                    _pColorBuffer[currPx] = _color;
                                }
                                dist = 0;
                            }
                        }
                    }
                }
                currPx++;
                wPx++;
                if(hPx == _windowSystem->GetMainWindow()->_height)
                    hPx = 0;
                if(wPx == _windowSystem->GetMainWindow()->_width){
                    hPx++;
                    wPx = 0;
                }
            }
            // end frame pass
        }
    }
}

// Thread render task function
void RendererSystem::RenderTask(int tilex0, int tiley0, int tilex1, int tiley1)
{
    int currPx {tiley0*_windowSystem->GetMainWindow()->_width+tilex0};
    int wPx {tilex0};
    int hPx {tiley0};
    float dist {0};
    float w0, w1, w2 {0};
    int maxPx = (tiley1*_windowSystem->GetMainWindow()->_width+tilex1);
    // frame pass 
    while(currPx <= maxPx){
        _pColorBuffer[currPx] = color(0x13,0x33,0x37,0xff);
        if(renderShapes){
            for(int f = 0; f < _triangles.size();f++){
                if(_triangles.at(f).BoundingBox(wPx, hPx)){
                    if(_triangles.at(f).Barycentric(wPx, hPx, w0, w1, w2)){
                        dist = glm::distance((w0*_triangles.at(f)._v[0].z+w1*_triangles.at(f)._v[1].z+w2*_triangles.at(f)._v[2].z), _mainCamera._wPos.z);
                        if(dist < _pZBuffer[currPx]){
                            _pZBuffer[currPx] = dist;
                            _triangles.at(f).Rasterize(w0, w1, w2, _color);
                            _pColorBuffer[currPx] = _color;
                        }
                        dist = 0;
                    }
                }
            }
        }
        currPx++;
        wPx++;
        if(wPx > tilex1){
            wPx = tilex0;
            hPx++;
            currPx = hPx*_windowSystem->GetMainWindow()->_width+tilex0;
        }
    }
    // end frame pass
}

void RendererSystem::DrawLine(int x0, int y0, int x1, int y1, unsigned long* buf, int col)
{
    for(float t=0.0; t<1.0;t+=0.01){
        int x = x0 + (x1-x0)*t;
        int y = y0 + (y1-y0)*t;
        if(GetIndexInMatrix(_windowSystem->GetMainWindow()->_width, _windowSystem->GetMainWindow()->_height, x, y) > -1)
            buf[GetIndexInMatrix(_windowSystem->GetMainWindow()->_width, _windowSystem->GetMainWindow()->_height, x, y)] = col;
    }
}

void RendererSystem::DrawPoint(int x, int y, unsigned long* buf, int col)
{
    if(GetIndexInMatrix(_windowSystem->GetMainWindow()->_width, _windowSystem->GetMainWindow()->_height, x, y) > -1)
        buf[GetIndexInMatrix(_windowSystem->GetMainWindow()->_width, _windowSystem->GetMainWindow()->_height, x, y)] = col;
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

void RendererSystem::AddTriangle(Triangle triangle)
{
    _triangles.push_back(triangle);
}

void RendererSystem::ClearBuffers()
{
    memset(_pColorBuffer, 0, _windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height * 4);
    std::fill(_pZBuffer, _pZBuffer + (_windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height), std::numeric_limits<float>::infinity());
}

void RendererSystem::SwapBuffers()
{
    memcpy(_pBackBuffer, _pColorBuffer, _windowSystem->GetMainWindow()->_width * _windowSystem->GetMainWindow()->_height * 4);

    BitBlt(GetDC(_windowSystem->GetMainWindow()->GetHandle()), 0, 0, _windowSystem->GetMainWindow()->_width, _windowSystem->GetMainWindow()->_height, _deviceContext, 0, 0, SRCCOPY);
}

void RendererSystem::PollRecompute()
{
    int i {0};
    if(GetMainCamera()._recompute){
        while(i < _triangles.size()){
            _triangles.at(i).ComputeVertices();
            i++;
        }
        GetMainCamera()._recompute = false;
    }
}