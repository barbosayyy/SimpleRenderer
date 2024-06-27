#include "Camera.h"

Camera::Camera(glm::vec3 pos) : _wPos(pos), _near(0.1f), _far(1000.0f), _fov(45.0f) {
    _view = glm::lookAt(_wPos, glm::vec3(0,0,-1),glm::vec3(0,1.0f,0.0));
}