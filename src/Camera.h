#pragma once
#include "Core.h"
#include "glm/ext/matrix_transform.hpp"

class Camera {
    public:
        Camera(glm::vec3 pos);
        glm::vec3 _wPos;
        float _near;
        float _far;
        glm::mat4 _view;
        float _fov;
};