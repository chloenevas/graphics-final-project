#include "sphere.h"
#include "imagereader.h"
#include <iostream>

Sphere::Sphere(const glm::mat4& ctm, const SceneMaterial& material, const Image* image)
    : Shape(ctm, material, image), m_center(0,0,0), m_radius(0.5f) {
    m_inverseCTM = glm::inverse(m_ctm);
}

glm::vec3 Sphere::calcNormal(const glm::vec3 point) {
    // Transform point back to object space
    glm::vec3 objectSpacePoint = glm::vec3(m_inverseCTM * glm::vec4(point, 1.0f));

    // Calculate normal in object space
    glm::vec3 objectSpaceNormal = glm::normalize(objectSpacePoint);

    // Transform normal back to world space using the transpose of the inverse of CTM
    return glm::normalize(glm::vec3(glm::transpose(m_inverseCTM) * glm::vec4(objectSpaceNormal, 0.0f)));
}

// Method to calculate the intersection with a ray
bool Sphere::calcIntersection(const glm::vec3 rayOrigin, const glm::vec3 rayDirection, glm::vec3& intersectionPoint, float& t, float time) {

    glm::vec3 P = glm::vec3(m_inverseCTM * glm::vec4(rayOrigin, 1.0f));
    glm::vec3 d = glm::normalize(glm::vec3(m_inverseCTM * glm::vec4(rayDirection, 0.0f)));


    // sphere moves from 0, 0, 0, to 0, 2, 0

   // std::cout<<time<<std::endl;
    // glm::vec3 movingCenter = m_center + glm::vec3(0.0f, static_cast<float>(0.07), 0.0f)*time;
    glm::vec3 movingCenter = m_center + (glm::vec3(0.0f, 0.1f, 0.0f) - m_center)*((time - 0) / (1 - 0));
        //return position_start + (position_end - position_start) * ((t - t_start) / (t_end - t_start));

    // calculate vector from ray origin to moving sphere's center
    //P = P - movingCenter;

    float a = glm::dot(d, d);
    float b = 2.0f * glm::dot(P, d);
    float c = glm::dot(P, P) - m_radius * m_radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant >= 0) {
        float t1 = (-b + sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b - sqrt(discriminant)) / (2.0f * a);

        if (t1 >= 0 && t2 >= 0) {
            t = glm::min(t1, t2);
        } else if (t1 >= 0) {
            t = t1;
        } else if (t2 >= 0) {
            t = t2;
        } else {
            return false;
        }

        intersectionPoint = P + t * d;
        intersectionPoint = glm::vec3(m_ctm * glm::vec4(intersectionPoint, 1.0f));

        return true;
    }

    return false;
}

BoundingBox Sphere::getBoundingBox() {
    glm::vec3 minCorner = m_center - glm::vec3(m_radius);
    glm::vec3 maxCorner = m_center + glm::vec3(m_radius);
    glm::vec3 transformedMin = glm::vec3(m_ctm * glm::vec4(minCorner, 1.0f));
    glm::vec3 transformedMax = glm::vec3(m_ctm * glm::vec4(maxCorner, 1.0f));
    return BoundingBox(transformedMin, transformedMax);
}

double Sphere::surfaceArea() {
    return 4.0 * M_PI * m_radius * m_radius;
}

glm::vec3 Sphere::getTexture(const glm::vec3& intersection) {
    if (m_image == nullptr){
        return glm::vec3(0, 0, 0);
    }

    float u = 0.0f, v = 0.0f;
    glm::vec3 objInt = glm::vec3(m_inverseCTM * glm::vec4(intersection, 1.0f));

    float theta = std::atan2(objInt.z, objInt.x);
    float phi = std::asin(objInt.y * 2.0f);

    u = theta >= 0 ? 1.f - (theta / (2.f * M_PI)) : -theta / (2.f * M_PI);
    v = phi / M_PI + 0.5f;

    int x = static_cast<int>(std::floor(u * m_material.textureMap.repeatU * m_image->width)) % m_image->width;
    int y = static_cast<int>(std::floor((1 - v) * m_material.textureMap.repeatV * m_image->height)) % m_image->height;

    if (u == 1.0f) x = m_material.textureMap.repeatU * m_image->width - 1;
    if (v == 0.0f) y = m_material.textureMap.repeatV * m_image->height - 1;

    RGBA pixel = m_image->data[y * m_image->width + x];
    return glm::vec3(pixel.r / 255.0f, pixel.g / 255.0f, pixel.b / 255.0f);
}


void Sphere::id() {
    std::cout << "Sphere" << std::endl;
}
