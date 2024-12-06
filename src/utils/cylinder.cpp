#include "cylinder.h"
#include "imagereader.h"
#include <iostream>

// Constructor that takes the CTM (Cumulative Transformation Matrix)
Cylinder::Cylinder(const glm::mat4& ctm, const SceneMaterial& material, const Image* image)
    : Shape(ctm, material, image), m_center(glm::vec3(0,0,0)), m_height(1.0f), m_radius(0.5f) {
    m_inverseCTM = glm::inverse(m_ctm);
}

glm::vec3 Cylinder::calcNormal(const glm::vec3 point) {

    glm::vec3 transformedPoint = glm::vec3(m_inverseCTM * glm::vec4(point, 1.0f));
    glm::vec3 objectNormal;

    if (transformedPoint.y >= m_height / 2.0f - 1e-4f) {
        objectNormal = glm::vec3(0.0f, 1.0f, 0.0f);
    } else if (transformedPoint.y <= -m_height / 2.0f + 1e-4f) {
        objectNormal = glm::vec3(0.0f, -1.0f, 0.0f);
    } else {
        objectNormal = glm::normalize(glm::vec3(transformedPoint.x, 0.0f, transformedPoint.z));
    }

    return glm::normalize(glm::vec3(glm::transpose(m_inverseCTM) * glm::vec4(objectNormal, 0.0f)));
}


bool Cylinder::calcIntersection(const glm::vec3 rayOrigin, const glm::vec3 rayDirection, glm::vec3& intersectionPoint, float &t, float time) {
    glm::vec3 P = glm::vec3(m_inverseCTM * glm::vec4(rayOrigin, 1.0f));
    glm::vec3 d = glm::normalize(glm::vec3(m_inverseCTM * glm::vec4(rayDirection, 0.0f)));

    float a = d.x * d.x + d.z * d.z;
    float b = 2 * (P.x * d.x + P.z * d.z);
    float c = P.x * P.x + P.z * P.z - m_radius * m_radius;
    float discriminant = b * b - 4 * a * c;

    float closestT = std::numeric_limits<float>::max();
    bool hasIntersection = false;

    if (discriminant >= 0) {
        float t1 = (-b + sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b - sqrt(discriminant)) / (2.0f * a);

        if (t1 >= 0 || t2 >= 0) {
            float tBody = (t1 >= 0 && t2 >= 0) ? glm::min(t1, t2) : glm::max(t1, t2);

            if (tBody >= 0) {
                float y = P.y + tBody * d.y;
                if (y >= -m_height / 2.0f && y <= m_height / 2.0f) {
                    glm::vec3 bodyIntersection = P + tBody * d;
                    bodyIntersection = glm::vec3(m_ctm * glm::vec4(bodyIntersection, 1.0f));

                    if (tBody < closestT) {
                        closestT = tBody;
                        intersectionPoint = bodyIntersection;
                        hasIntersection = true;
                    }
                }
            }
        }
    }

    float topY = m_height / 2.0f;
    if (d.y != 0) {
        float tTop = (topY - P.y) / d.y;
        if (tTop >= 0) {
            glm::vec3 topIntersection = P + tTop * d;
            if (topIntersection.x * topIntersection.x + topIntersection.z * topIntersection.z <= m_radius * m_radius) {
                topIntersection = glm::vec3(m_ctm * glm::vec4(topIntersection, 1.0f));

                if (tTop < closestT) {
                    closestT = tTop;
                    intersectionPoint = topIntersection;
                    hasIntersection = true;
                }
            }
        }
    }

    float bottomY = -m_height / 2.0f;
    if (d.y != 0) {
        float tBottom = (bottomY - P.y) / d.y;
        if (tBottom >= 0) {
            glm::vec3 bottomIntersection = P + tBottom * d;
            if (bottomIntersection.x * bottomIntersection.x + bottomIntersection.z * bottomIntersection.z <= m_radius * m_radius) {
                bottomIntersection = glm::vec3(m_ctm * glm::vec4(bottomIntersection, 1.0f));

                if (tBottom < closestT) {
                    closestT = tBottom;
                    intersectionPoint = bottomIntersection;
                    hasIntersection = true;
                }
            }
        }
    }

    if (hasIntersection) {
        t = closestT;
    }
    return hasIntersection;
}

BoundingBox Cylinder::getBoundingBox() {
    glm::vec3 minCorner = m_center - glm::vec3(m_radius, m_height/2, m_radius);
    glm::vec3 maxCorner = m_center + glm::vec3(m_radius, m_height/2, m_radius);
    glm::vec3 transformedMin = glm::vec3(m_ctm * glm::vec4(minCorner, 1.0f));
    glm::vec3 transformedMax = glm::vec3(m_ctm * glm::vec4(maxCorner, 1.0f));
    return BoundingBox(transformedMin, transformedMax);
}

double Cylinder::surfaceArea() {
    return 2.0 * M_PI * m_radius * (m_radius + m_height);
}

glm::vec3 Cylinder::getTexture(const glm::vec3& intersection) {
    if (m_image == nullptr){
        return glm::vec3(0, 0, 0);
    }

    float u = 0.0f, v = 0.0f;
    glm::vec3 objInt = glm::vec3(m_inverseCTM * glm::vec4(intersection, 1.0f));

    if (objInt.y >= (m_height / 2.0f) - 1e-4f){
        u = (objInt.x + 0.5);
        v = (-objInt.z + 0.5);
    }
    else if (objInt.y <= (-m_height / 2.0f) + 1e-4f){
        u = (objInt.x + 0.5);
        v = (objInt.z + 0.5);
    }
    else {
        v = objInt.y + 0.5f;
        float theta = std::atan2(objInt.z, objInt.x);
        u = theta >= 0 ? 1.f - (theta / (2.f * M_PI)) : -theta / (2.f * M_PI);
    }

    int x = static_cast<int>(std::floor(u * m_material.textureMap.repeatU * m_image->width)) % m_image->width;
    int y = static_cast<int>(std::floor((1 - v) * m_material.textureMap.repeatV * m_image->height)) % m_image->height;

    if (u == 1.0f) x = m_material.textureMap.repeatU * m_image->width - 1;
    if (v == 0.0f) y = m_material.textureMap.repeatV * m_image->height - 1;

    RGBA pixel = m_image->data[y * m_image->width + x];
    return glm::vec3(pixel.r / 255.0f, pixel.g / 255.0f, pixel.b / 255.0f);
}


void Cylinder::id(){
    std::cout << "Sphere" << std::endl;
}
