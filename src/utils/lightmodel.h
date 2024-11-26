#pragma once

#include <glm/glm.hpp>
#include "utils/scenedata.h"
#include "raytracer/raytracescene.h"
#include "rgba.h"



glm::vec4 phong(const RayTraceScene &scene,
           glm::vec3  position,
           glm::vec3  normal,
           glm::vec3  directionToCamera,
           SceneMaterial material,
           SceneLightData light,
           glm::vec3 texture);
