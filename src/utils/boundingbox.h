#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;

    BoundingBox(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

    bool traces(const glm::vec3& origin, const glm::vec3& direction) const {
        float tMin = (min.x - origin.x) / direction.x;
        float tMax = (max.x - origin.x) / direction.x;

        if (tMin > tMax) std::swap(tMin, tMax);

        float tyMin = (min.y - origin.y) / direction.y;
        float tyMax = (max.y - origin.y) / direction.y;
        if (tyMin > tyMax) std::swap(tyMin, tyMax);

        if ((tMin > tyMax) || (tyMin > tMax)) return false;
        tMin = std::fmax(tMin, tyMin);
        tMax = std::fmin(tMax, tyMax);

        float tzMin = (min.z - origin.z) / direction.z;
        float tzMax = (max.z - origin.z) / direction.z;
        if (tzMin > tzMax) std::swap(tzMin, tzMax);

        if ((tMin > tzMax) || (tzMin > tMax)) return false;
        tMin = std::fmax(tMin, tzMin);
        tMax = std::fmin(tMax, tzMax);

        return tMax >= std::fmax(0.0f, tMin);
    }

    bool intersects(const BoundingBox& other) const {
        bool overlapX = (min.x < other.max.x) && (max.x > other.min.x);
        bool overlapY = (min.y < other.max.y) && (max.y > other.min.y);
        bool overlapZ = (min.z < other.max.z) && (max.z > other.min.z);

        return overlapX || overlapY || overlapZ;
    }

    bool contains(const BoundingBox& other) const {
        return (min.x <= other.min.x && max.x >= other.max.x) &&
               (min.y <= other.min.y && max.y >= other.max.y) &&
               (min.z <= other.min.z && max.z >= other.max.z);
    }

    float surfaceArea() const {
        float dx = std::abs(max.x - min.x);
        float dy = std::abs(max.y - min.y);
        float dz = std::abs(max.z - min.z);

        return 2.0f * (dx * dy + dy * dz + dz * dx);
    }

};

#endif // BOUNDINGBOX_H
