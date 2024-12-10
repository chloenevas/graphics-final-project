#pragma once

#include "shape.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "boundingbox.h"

class Cube : public Shape {
public:
    Cube(const glm::mat4& ctm, const SceneMaterial& material, const Image* image);

    glm::vec3 calcNormal(const glm::vec3 point, float time) override;

    bool calcIntersection(const glm::vec3 rayOrigin, const glm::vec3 rayDirection, glm::vec3& intersectionPoint, float& t, float time) override;

    BoundingBox getBoundingBox() override;

    double surfaceArea() override;

    glm::vec3 getTexture(const glm::vec3& intersection) override;

    void id() override;

private:
    glm::vec3 m_center;
    float m_length;
};
