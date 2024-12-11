#include "cube.h"
#include <limits>
#include <iostream>

Cube::Cube(const glm::mat4& ctm, const SceneMaterial& material, glm::vec3 velocity, const Image* image)
    : Shape(ctm, material, velocity, image), m_center(glm::vec3(0,0,0)), m_length(1.0f) {
    m_inverseCTM = glm::inverse(m_ctm);
}

glm::vec3 Cube::calcNormal(const glm::vec3 point) {
    glm::vec3 transformedPoint = glm::vec3(m_inverseCTM * glm::vec4(point, 1.0f));

    glm::vec3 objectNormal;
    glm::vec3 absPoint = glm::abs(transformedPoint);

    // Determine the axis of the normal based on the largest absolute component
    if (absPoint.x > absPoint.y && absPoint.x > absPoint.z) {
        objectNormal = glm::vec3(transformedPoint.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
    } else if (absPoint.y > absPoint.x && absPoint.y > absPoint.z) {
        objectNormal = glm::vec3(0.0f, transformedPoint.y > 0 ? 1.0f : -1.0f, 0.0f);
    } else {
        objectNormal = glm::vec3(0.0f, 0.0f, transformedPoint.z > 0 ? 1.0f : -1.0f);
    }

    return glm::normalize(glm::vec3(glm::transpose(m_inverseCTM) * glm::vec4(objectNormal, 0.0f)));
}


// Method to calculate the intersection with a ray
bool Cube::calcIntersection(const glm::vec3 rayOrigin, const glm::vec3 rayDirection, glm::vec3& intersectionPoint, float &t, float time, float vel) {
    glm::vec3 P = glm::vec3(m_inverseCTM * glm::vec4(rayOrigin, 1.0f));
    glm::vec3 d = glm::normalize(glm::vec3(m_inverseCTM * glm::vec4(rayDirection, 0.0f)));

    // calculate vector from ray origin to moving sphere's center
    glm::vec3 movingCenter = m_center + time * m_velocity;
    P = P - movingCenter;

    float tMin = std::numeric_limits<float>::infinity();

    for (int i = 0; i < 3; i++) {
        if (d[i] != 0) {
            float t1 = ((-0.5f) - P[i]) / d[i];
            float t2 = ((0.5f) - P[i]) / d[i];

            if (t1 > t2) std::swap(t1, t2);

            glm::vec3 P1 = P + t1 * d;
            glm::vec3 P2 = P + t2 * d;

            bool validAtT1 = (t1 > 0 && -0.5f <= P1[(i + 1) % 3]) && (P1[(i + 1) % 3] <= 0.5f) &&
                             (-0.5f <= P1[(i + 2) % 3]) && (P1[(i + 2) % 3] <= 0.5f);
            bool validAtT2 = (t2 > 0 && -0.5f <= P2[(i + 1) % 3]) && (P2[(i + 1) % 3] <= 0.5f) &&
                             (-0.5f <= P2[(i + 2) % 3]) && (P2[(i + 2) % 3] <= 0.5f);

            if (validAtT1) {
                tMin = glm::min(tMin, t1);
            }
            else if (validAtT2) {
                tMin = glm::min(tMin, t2);
            }
        }
    }

    if (tMin >= std::numeric_limits<float>::infinity()) {
        return false;
    }

    t = tMin;
    intersectionPoint = P + t * d;
    intersectionPoint = glm::vec3(m_ctm * glm::vec4(intersectionPoint, 1.0f));
    return true;
}

BoundingBox Cube::getBoundingBox() {
    float halfSide = m_length / 2.0f;
    glm::vec3 minCorner = m_center - glm::vec3(halfSide);
    glm::vec3 maxCorner = m_center + glm::vec3(halfSide);
    glm::vec3 transformedMin = glm::vec3(m_ctm * glm::vec4(minCorner, 1.0f));
    glm::vec3 transformedMax = glm::vec3(m_ctm * glm::vec4(maxCorner, 1.0f));
    return BoundingBox(transformedMin, transformedMax);
}

double Cube::surfaceArea() {
    return 6.0 * m_length * m_length;
}

glm::vec3 Cube::getTexture(const glm::vec3& intersection) {
    if (m_image == nullptr){
        return glm::vec3(0, 0, 0);
    }

    float u = 0.0f, v = 0.0f;
    glm::vec3 objInt = glm::vec3(m_inverseCTM * glm::vec4(intersection, 1.0f));

    if (fabs(objInt.x) > fabs(objInt.y) && fabs(objInt.x) > fabs(objInt.z)) {
        if (objInt.x > 0) {
            u = (-objInt.z + 0.5f);
            v = (objInt.y + 0.5f);
        } else {
            u = (objInt.z + 0.5f);
            v = (objInt.y + 0.5f);
        }
    } else if (fabs(objInt.y) > fabs(objInt.x) && fabs(objInt.y) > fabs(objInt.z)) {
        if (objInt.y > 0) {
            u = (objInt.x + 0.5f);
            v = (-objInt.z + 0.5f);
        } else {
            u = (objInt.x + 0.5f);
            v = (objInt.z + 0.5f);
        }
    } else {
        if (objInt.z > 0) {
            u = (objInt.x + 0.5f);
            v = (objInt.y + 0.5f);
        } else {
            u = (-objInt.x + 0.5f);
            v = (objInt.y + 0.5f);
        }
    }

    int x = static_cast<int>(std::floor(u * m_material.textureMap.repeatU * m_image->width)) % m_image->width;
    int y = static_cast<int>(std::floor((1 - v) * m_material.textureMap.repeatV * m_image->height)) % m_image->height;

    if (u == 1.0f) x = m_material.textureMap.repeatU * m_image->width - 1;
    if (v == 0.0f) y = m_material.textureMap.repeatV * m_image->height - 1;

    RGBA pixel = m_image->data[y * m_image->width + x];
    return glm::vec3(pixel.r / 255.0f, pixel.g / 255.0f, pixel.b / 255.0f);
}


void Cube::id(){
    // std::cout << "Cube" << std::endl;
}
