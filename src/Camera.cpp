#include "Camera.h"

Camera::Camera(glm::vec3 pos) : _wPos(pos), _near(0.1f), _far(1000.0f), _fov(45.0f), _recompute(false) {
    _view = glm::lookAt(_wPos, glm::vec3(0,0,-1),glm::vec3(0,1.0f,0.0));
}

void Camera::RegisterInputSystem(IInputSystem* inputSystem)
{
    _inputSystem = inputSystem;
    std::function<void()> func = std::bind(&Camera::Move, this);
    _inputSystem->RegisterAction(func);
}

void Camera::Move() {
    if(_inputSystem->GetKeyPressDown('W')){
        glm::vec3 wPos = glm::vec3(_wPos.x, _wPos.y, _wPos.z -= 1.0);
        glm::mat4 view = glm::lookAt(wPos, glm::vec3(_wPos.x,_wPos.y,-1),glm::vec3(0,1.0f,0.0));
        _view = view;
        _recompute = true;
    }
    else if(_inputSystem->GetKeyPressDown('A')){
        glm::vec3 wPos = glm::vec3(_wPos.x -= 0.5, _wPos.y, _wPos.z);
        glm::mat4 view = glm::lookAt(wPos, glm::vec3(0,0,-1),glm::vec3(0,1.0f,0.0));
        _view = view;
        _recompute = true;
    }
    else if(_inputSystem->GetKeyPressDown('D')){
        glm::vec3 wPos = glm::vec3(_wPos.x += 0.5, _wPos.y, _wPos.z);
        glm::mat4 view = glm::lookAt(wPos, glm::vec3(0,0,-1),glm::vec3(0,1.0f,0.0));
        _view = view;
        _recompute = true;
    }
    else if(_inputSystem->GetKeyPressDown('S')){
        glm::vec3 wPos = glm::vec3(_wPos.x, _wPos.y, _wPos.z += 1.0);
        glm::mat4 view = glm::lookAt(wPos, glm::vec3(_wPos.x,_wPos.y,-1),glm::vec3(0,1.0f,0.0));
        _view = view;
        _recompute = true;
    }
}