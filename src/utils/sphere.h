#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "scenedata.h"

class Sphere : public Shape {
public:
    Sphere(const glm::mat4& ctm, const SceneMaterial& material, const Image* image);

    bool calcIntersection(const glm::vec3 rayOrigin, const glm::vec3 rayDirection, glm::vec3& intersectionPoint, float& t, float time) override;

    glm::vec3 calcNormal(const glm::vec3 point, float time) override;

    BoundingBox getBoundingBox() override;

    double surfaceArea() override;

    glm::vec3 getTexture(const glm::vec3& intersection) override;

    void id() override;

private:
    glm::vec3 m_center;
    float m_radius;
};

#endif // SPHERE_H
