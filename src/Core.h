#pragma once

#include <vector>
#include "glm/glm.hpp"

constexpr unsigned int ARGB_RED = 0x00FF0000;
constexpr unsigned int ARGB_GREEN = 0x0000FF00;
constexpr unsigned int ARGB_BLUE = 0x000000FF;
constexpr unsigned int ARGB_DARKTEAL = 0x00133337;

constexpr int cfgDefaultWindowWidth = 1024;
constexpr int cfgDefaultWindowHeight = 768;

using Vector3 = glm::vec3;
using Vector2 = glm::vec2; 