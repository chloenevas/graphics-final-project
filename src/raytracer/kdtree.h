#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <glm/glm.hpp>
#include "utils/shape.h"
#include "utils/boundingbox.h"

class KdTree {

public:
    struct KdNode {
        BoundingBox box;
        std::vector<Shape*> shapes;
        int axis;
        KdNode* left;
        KdNode* right;

        KdNode(const BoundingBox& box, const std::vector<Shape*>& shapes, int axis)
            : box(box), shapes(shapes), axis(axis), left(nullptr), right(nullptr) {}
    };
public:
    // KdNode* root = nullptr;

    KdNode* build(std::vector<Shape*>& shapes, const BoundingBox& parentBox, int depth = 0);
    // KdNode* insert(KdNode* node, Shape* shape, int depth = 0);
    std::vector<Shape*> query(const glm::vec3& origin, const glm::vec3& direction, KdNode* node);

};

#endif // KDTREE_H

