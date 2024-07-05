#include "Primitives.h"
#include "WindowSystem.h"
#include "Math.h"
#include "RendererSystem.h"

#include "Console.h"

#include "glm/gtc/matrix_transform.hpp"

extern RendererSystem rendererSystem;
extern WindowSystem windowSystem;

Triangle::Triangle(Vector3 v0, Vector3 v1, Vector3 v2) : _minX(rendererSystem.GetWindowSystem()->GetMainWindow()->_width), _minY(rendererSystem.GetWindowSystem()->GetMainWindow()->_height), _maxX(0), _maxY(0){
    _v[0] = v0;
    _v[1] = v1;
    _v[2] = v2;

    ComputeVertices();
    PrintProjectedCorners();
}

void Triangle::ComputeVertices(){
    _projected[0] = glm::vec4(0,0,0,1.0f);
    _projected[1] = glm::vec4(0,0,0,1.0f);
    _projected[2] = glm::vec4(0,0,0,1.0f);

    glm::vec4 modelWorldSpace[3];
    glm::vec4 cameraSpace[3];
    glm::vec4 screenSpace[3];
    glm::vec3 ndc[3];
    glm::vec4 raster[3];

    for(int i = 0; i < 3; i++){
        modelWorldSpace[i] = glm::mat4(1.0f) * glm::vec4(_v[i].x, _v[i].y, _v[i].z, 1.0f);
        cameraSpace[i] = rendererSystem.GetMainCamera()._view * modelWorldSpace[i];
        screenSpace[i] = glm::perspective(glm::radians(45.0f), (float)windowSystem.GetMainWindow()->_width/(float)windowSystem.GetMainWindow()->_height, rendererSystem.GetMainCamera()._near, rendererSystem.GetMainCamera()._far) * cameraSpace[i];
        
        if(screenSpace[i].w != 0.0f){
            ndc[i] = glm::vec3(screenSpace[i]) / screenSpace[i].w;
        }
        else{
            ndc[i] = glm::vec3(screenSpace[i]);
        }

        float x = (ndc[i].x+1.0f) * 0.5f * windowSystem.GetMainWindow()->_width;
        float y = (1.0f-ndc[i].y) * 0.5f * windowSystem.GetMainWindow()->_height;
        _projected[i] = glm::vec4(x, y, ndc[i].z, 1.0f);

        if(_projected[i].x < _minX){
            _minX = std::max(0, std::min(windowSystem.GetMainWindow()->_width-1, (int)_projected[i].x));
        }
        if(_projected[i].y < _minY){
            _minY = std::max(0, std::min(windowSystem.GetMainWindow()->_height-1, (int)_projected[i].y));
        }
        if(_projected[i].x > _maxX){
            _maxX = std::max(0, std::min(windowSystem.GetMainWindow()->_width-1, (int)_projected[i].x));
        }
        if(_projected[i].y > _maxY){
            _maxY = std::max(0, std::min(windowSystem.GetMainWindow()->_height-1, (int)_projected[i].y));
        }
    }
}

bool Triangle::BoundingBox(int x, int y)
{
    if(x > _minX && x < _maxX && y > _minY && y < _maxY)
        return true;
    else 
        return false;
}

bool Triangle::Barycentric(int pX, int pY, float &w0, float &w1, float &w2)
{
    Vector2 pV2[3] = {Vector2(_projected[0].x, _projected[0].y), Vector2(_projected[1].x, _projected[1].y), Vector2(_projected[2].x, _projected[2].y)};
    float area = EdgeInverted(pV2[0], pV2[1], pV2[2]);
    w0 = EdgeInverted(Vector2(pX,pY), pV2[1], pV2[2])/area;
    w1 = EdgeInverted(Vector2(pX,pY), pV2[2], pV2[0])/area;
    w2 = EdgeInverted(Vector2(pX,pY), pV2[0], pV2[1])/area;
    if(w0 >= 0 && w1 >= 0 && w2 >= 0){
        return true;
    }
    else{
        return false;
    }
}

void Triangle::Rasterize(float w0, float w1, float w2, int& col){
    col = color((w0*1+w1*0+w2*0)*255,(w0*0+w1*1+w2*0)*255,(w0*0+w1*0+w2*1)*255,255);
}

void Triangle::PrintProjectedCorners(){
    Console::Out("Triangle projected points:");
    for(int i = 0; i< 3; i++){
        Console::Out(_projected[i].x, " ", _projected[i].y);
    }
    Console::Out("minX maxX minY maxY: ", _minX, " ", _maxX, " ", _minY, " ", _maxY);
}