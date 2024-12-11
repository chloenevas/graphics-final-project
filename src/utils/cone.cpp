#include "cone.h"
#include <iostream>

Cone::Cone(const glm::mat4& ctm, const SceneMaterial& material, glm::vec3 velocity, const Image* image)
    : Shape(ctm, material, velocity, image), m_center(glm::vec3(0,0,0)), m_height(1.0f), m_radius(0.5f) {
    m_inverseCTM = glm::inverse(m_ctm);
}

glm::vec3 Cone::calcNormal(const glm::vec3 point) {

    glm::vec3 transformedPoint = glm::vec3(m_inverseCTM * glm::vec4(point, 1.0f));
    glm::vec3 objectNormal;

    if (transformedPoint.y <= -m_height / 2.0f + 1e-4f) {
        objectNormal = glm::vec3(0.0f, -1.0f, 0.0f);
    }
    else {
        float slope = m_radius / m_height;
        objectNormal = glm::normalize(glm::vec3(transformedPoint.x, -slope * slope * (transformedPoint.y - (m_height/2)), transformedPoint.z));
    }
    return glm::normalize(glm::vec3(glm::transpose(m_inverseCTM) * glm::vec4(objectNormal, 0.0f)));
}


// Method to calculate the intersection with a ray
bool Cone::calcIntersection(const glm::vec3 rayOrigin, const glm::vec3 rayDirection, glm::vec3& intersectionPoint, float& t, float time, float vel) {
    glm::vec3 P = glm::vec3(m_inverseCTM * glm::vec4(rayOrigin, 1.0f));
    glm::vec3 d = glm::normalize(glm::vec3(m_inverseCTM * glm::vec4(rayDirection, 0.0f)));

    // calculate vector from ray origin to moving sphere's center
    glm::vec3 movingCenter = m_center + time * m_velocity;
    P = P - movingCenter;

    float k = (m_radius * m_radius) / (m_height * m_height);

    float a = d.x * d.x + d.z * d.z - k * d.y * d.y;
    float b = 2.0f * (P.x * d.x + P.z * d.z - k * P.y * d.y + (k / 2.0f) * d.y);
    float c = P.x * P.x + P.z * P.z - k * P.y * P.y + k * P.y - (k / 4);

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    }


    float t1 = (-b + sqrt(discriminant)) / (2.0f * a);
    float t2 = (-b - sqrt(discriminant)) / (2.0f * a);

    bool t1Valid = t1 >= 0;
    bool t2Valid = t2 >= 0;

    if (t1Valid) {
        float y1 = P.y + t1 * d.y;
        t1Valid = (y1 >= -m_height / 2.0f && y1 <= m_height / 2.0f);
    }

    if (t2Valid) {
        float y2 = P.y + t2 * d.y;
        t2Valid = (y2 >= -m_height / 2.0f && y2 <= m_height / 2.0f);
    }

    float tCone = -1.0f;
    if (t1Valid && t2Valid) {
        tCone = glm::min(t1, t2);
    } else if (t1Valid) {
        tCone = t1;
    } else if (t2Valid) {
        tCone = t2;
    }

    bool intersectsCone = (tCone >= 0);

    float tCap = -1.0f;
    glm::vec3 intersectionCap;
    bool intersectsCap = false;

    if (d.y != 0) {
        tCap = (-m_height / 2.0f - P.y) / d.y;
        if (tCap >= 0) {
            glm::vec3 potentialCap = P + tCap * d;
            if (potentialCap.x * potentialCap.x + potentialCap.z * potentialCap.z <= m_radius * m_radius) {
                intersectsCap = true;
            }
        }
    }

    if (intersectsCone && intersectsCap) {
        t = glm::min(tCone, tCap);
        intersectionPoint = P + t * d;
        intersectionPoint = glm::vec3(m_ctm * glm::vec4(intersectionPoint, 1.0f));
        return true;
    } else if (intersectsCone) {
        intersectionPoint = P + tCone * d;
        intersectionPoint = glm::vec3(m_ctm * glm::vec4(intersectionPoint, 1.0f));
        t = tCone;
        return true;
    } else if (intersectsCap) {
        intersectionPoint = P + tCap * d;
        intersectionPoint = glm::vec3(m_ctm * glm::vec4(intersectionPoint, 1.0f));;
        t = tCap;
        return true;
    }

    return false;
}


BoundingBox Cone::getBoundingBox() {
    glm::vec3 minCorner = m_center - glm::vec3(m_radius, m_height/2, m_radius);
    glm::vec3 maxCorner = m_center + glm::vec3(m_radius, m_height/2, m_radius);
    glm::vec3 transformedMin = glm::vec3(m_ctm * glm::vec4(minCorner, 1.0f));
    glm::vec3 transformedMax = glm::vec3(m_ctm * glm::vec4(maxCorner, 1.0f));
    return BoundingBox(transformedMin, transformedMax);
}

double Cone::surfaceArea() {
    double slantHeight = sqrt(m_radius * m_radius + m_height * m_height);
    return M_PI * m_radius * (m_radius + slantHeight);
}

glm::vec3 Cone::getTexture(const glm::vec3& intersection) {
    if (m_image == nullptr){
        return glm::vec3(0, 0, 0);
    }
    float u = 0.0f, v = 0.0f;
    glm::vec3 objInt = glm::vec3(m_inverseCTM * glm::vec4(intersection, 1.0f));

    if (objInt.y <= + (-m_height / 2) + 1e-4f){
        u = (objInt.x + 0.5);
        v = (objInt.z + 0.5);
    }
    else{
        float theta = std::atan2(objInt.z, objInt.x);
        u = theta >= 0 ? 1.f - (theta / (2.f * M_PI)) : -theta / (2.f * M_PI);
        v = objInt.y + 0.5f;
    }

    int x = static_cast<int>(std::floor(u * m_material.textureMap.repeatU * m_image->width)) % m_image->width;
    int y = static_cast<int>(std::floor((1 - v) * m_material.textureMap.repeatV * m_image->height)) % m_image->height;

    if (u == 1.0f) x = m_material.textureMap.repeatU * m_image->width - 1;
    if (v == 0.0f) y = m_material.textureMap.repeatV * m_image->height - 1;

    RGBA pixel = m_image->data[y * m_image->width + x];

    return glm::vec3(pixel.r / 255.0f, pixel.g / 255.0f, pixel.b / 255.0f);
}

void Cone::id() {
    std::cout << "Cone" << std::endl;
}
