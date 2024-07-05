#pragma once
#include "Core.h"
#include "glm/ext/matrix_transform.hpp"
#include "IInputSystem.h"

class Camera {
    public:
        Camera(glm::vec3 pos);
        ~Camera() {};

        glm::vec3 _wPos;
        float _near;
        float _far;
        glm::mat4 _view;
        float _fov;
        bool _recompute;
        void RegisterInputSystem(IInputSystem* inputSystem);
        void Move();
    private:
        IInputSystem* _inputSystem;
};