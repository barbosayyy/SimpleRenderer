#include "Primitives.h"
#include "Windowing.h"
#include "Math.h"
#include "Renderer.h"
#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

extern RendererSystem rendererSystem;

Triangle::Triangle(Vector3 v0, Vector3 v1, Vector3 v2) : _minX(rendererSystem.GetWindowSystem()->GetMainWindow()->_width), _minY(rendererSystem.GetWindowSystem()->GetMainWindow()->_height), _maxX(0), _maxY(0){
    _v[0] = v0;
    _v[1] = v1;
    _v[2] = v2;
    // glm::vec4 pCameraSpace[3];
    // glm::vec2 pScreen[3] {glm::vec2(0,0),glm::vec2(0,0),glm::vec2(0,0)};
    // glm::vec2 pNDC[3] {glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0)};

    _projected[0] = glm::vec4(0,0,0,1.0f);
    _projected[1] = glm::vec4(0,0,0,1.0f);
    _projected[2] = glm::vec4(0,0,0,1.0f);

    glm::vec4 modelWorldSpace[3] = {glm::vec4(0), glm::vec4(0), glm::vec4(0)};
    glm::vec4 cameraSpace[3]  = {glm::vec4(0), glm::vec4(0), glm::vec4(0)};
    glm::vec4 screenSpace[3]  = {glm::vec4(0), glm::vec4(0), glm::vec4(0)};
    glm::vec3 ndc[3]  = {glm::vec3(0), glm::vec3(0), glm::vec3(0)};
    glm::vec4 raster[3]  = {glm::vec4(0), glm::vec4(0), glm::vec4(0)};

    for(int i = 0; i < 3; i++){
        // pCameraSpace[i] = rendererSystem.GetMainCamera()._view * glm::vec4(_v[i].x, _v[i].y, _v[i].z, 1.0f);
        // pScreen[i].x = rendererSystem.GetMainCamera()._near * pCameraSpace[i].x / -pCameraSpace[i].z;
        // pScreen[i].y = rendererSystem.GetMainCamera()._near * pCameraSpace[i].y / -pCameraSpace[i].z;
        modelWorldSpace[i] = glm::mat4(1.0f) * glm::vec4(_v[i].x, _v[i].y, _v[i].z, 1.0f);
        cameraSpace[i] = rendererSystem.GetMainCamera()._view * modelWorldSpace[i];
        screenSpace[i] = glm::perspective(glm::radians(45.0f), 1024.0f/768.0f, rendererSystem.GetMainCamera()._near, rendererSystem.GetMainCamera()._far) * cameraSpace[i];

        std::cout << cameraSpace[i].x << cameraSpace[i].y << cameraSpace[i].z << std::endl;
        
        if(screenSpace[i].w != 0.0f){
            ndc[i] = glm::vec3(screenSpace[i]) / screenSpace[i].w;
        }
        else{
            ndc[i] = glm::vec3(screenSpace[i]);
        }

        float x = (ndc[i].x+1.0f) * 0.5f * 1024;
        float y = (1.0f-ndc[i].y) * 0.5f * 768;
        _projected[i] = glm::vec4(x, y, ndc[i].z, 1.0f);

            if(_projected[i].x < _minX){
                _minX = std::max(0, std::min(cfgDefaultWindowWidth-1, (int)_projected[i].x));
            }
            if(_projected[i].y < _minY){
                _minY = std::max(0, std::min(cfgDefaultWindowHeight-1, (int)_projected[i].y));
            }
            if(_projected[i].x > _maxX){
                _maxX = std::max(0, std::min(cfgDefaultWindowWidth-1, (int)_projected[i].x));
            }
            if(_projected[i].y > _maxY){
                _maxY = std::max(0, std::min(cfgDefaultWindowHeight-1, (int)_projected[i].y));
            }
    }

    // for(int i=0;i<3;i++){
    //     float x_proj = _v[i].x;
    //     float y_proj = _v[i].y;
    //     float x_proj_remap = (1+x_proj)/2;
    //     float y_proj_remap = (1+y_proj)/2;
    //     _pV[i].x = x_proj_remap * cfgDefaultWindowWidth;
    //     _pV[i].y = y_proj_remap * cfgDefaultWindowHeight;

    //     if(_pV[i].x < _minX){
    //         _minX = std::max(0, std::min(cfgDefaultWindowWidth-1, (int)_pV[i].x));
    //     }
    //     if(_pV[i].y < _minY){
    //         _minY = std::max(0, std::min(cfgDefaultWindowHeight-1, (int)_pV[i].y));
    //     }
    //     if(_pV[i].x > _maxX){
    //         _maxX = std::max(0, std::min(cfgDefaultWindowWidth-1, (int)_pV[i].x));
    //     }
    //     if(_pV[i].y > _maxY){
    //         _maxY = std::max(0, std::min(cfgDefaultWindowHeight-1, (int)_pV[i].y));
    //     }
    // }
    PrintProjectedCorners();
}

bool Triangle::Rasterize(int x, int y, int& color){
    if(x > _minX && x < _maxX && y > _minY && y < _maxY){
        Vector2 pV2[3] = {Vector2(_projected[0].x, _projected[0].y), Vector2(_projected[1].x, _projected[1].y), Vector2(_projected[2].x, _projected[2].y)};
        float w0 = EdgeInverted(Vector2(x,y), pV2[1], pV2[2])/EdgeInverted(pV2[0], pV2[1], pV2[2]);
        float w1 = EdgeInverted(Vector2(x,y), pV2[2], pV2[0])/EdgeInverted(pV2[0], pV2[1], pV2[2]);
        float w2 = EdgeInverted(Vector2(x,y), pV2[0], pV2[1])/EdgeInverted(pV2[0], pV2[1], pV2[2]);
        if(w0 >= 0 && w1 >= 0 && w2 >= 0){
            color = Color((w0*1+w1*0+w2*0)*255,(w0*0+w1*1+w2*0)*255,(w0*0+w1*0+w2*1)*255,255);
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

void Triangle::PrintProjectedCorners(){
    std::cout << "Triangle projected points:" << std::endl;
    for(int i = 0; i< 3; i++){
        std::cout << _projected[i].x << " " << _projected[i].y << std::endl;
    }
    std::cout << "minX maxX minY maxY: " << _minX << " " << _maxX << " " << _minY << " " << _maxY << std::endl;
}