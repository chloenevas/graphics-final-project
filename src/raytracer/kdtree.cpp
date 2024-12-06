#include "kdtree.h"
#include <iostream>

KdTree::KdNode* KdTree::build(std::vector<Shape*>& shapes, const BoundingBox& parentBox, int depth) {

    int axis = depth % 3;

    if (shapes.size() < 4 || depth == 0) {
        return new KdTree::KdNode(parentBox, shapes, axis);
    }

    if (shapes.empty()) {
        return nullptr;
    }

    float bestCost = std::numeric_limits<float>::infinity();
    float bestSplit = 0.0f;

    float totalShapeSurfaceArea = parentBox.surfaceArea();

    for (int i = 0; i < shapes.size() - 1; i++) {
        float splitValue = shapes[i]->getBoundingBox().max[axis];

        BoundingBox leftBox = parentBox;
        BoundingBox rightBox = parentBox;

        leftBox.max[axis] = splitValue;
        rightBox.min[axis] = splitValue;

        float leftShapeSurfaceArea = leftBox.surfaceArea();
        float rightShapeSurfaceArea = rightBox.surfaceArea();

        float weightLeft = leftShapeSurfaceArea / totalShapeSurfaceArea;
        float weightRight = rightShapeSurfaceArea / totalShapeSurfaceArea;

        float cost = weightLeft * leftShapeSurfaceArea + weightRight * rightShapeSurfaceArea;

        if (cost < bestCost) {
            bestCost = cost;
            bestSplit = splitValue;
        }
    }

    BoundingBox leftBox = parentBox;
    BoundingBox rightBox = parentBox;

    leftBox.max[axis] = bestSplit;
    rightBox.min[axis] = bestSplit;

    std::vector<Shape*> leftShapes, rightShapes;

    for (Shape* shape : shapes) {
        BoundingBox shapeBox = shape->getBoundingBox();

        if (leftBox.intersects(shapeBox) || leftBox.contains(shapeBox)) {
            leftShapes.push_back(shape);
        }

        if (rightBox.intersects(shapeBox) || rightBox.contains(shapeBox)) {
            rightShapes.push_back(shape);
        }
    }

    KdNode* node = new KdTree::KdNode(parentBox, shapes, axis);
    node->left = build(leftShapes, leftBox, depth + 1);
    node->right = build(rightShapes, rightBox, depth + 1);

    return node;
}

std::vector<Shape*> KdTree::query(const glm::vec3& origin, const glm::vec3& direction, KdTree::KdNode* node) {


    return node->shapes;

    // std::vector<Shape*> result;
    // if (!node) return result;

    // if (!node->box.traces(origin, direction)) {
    //     return result;
    // }

    // if (node->left == nullptr && node->right == nullptr) {
    //     result.insert(result.end(), node->shapes.begin(), node->shapes.end());
    //     return result;
    // }

    // if (node->left) {
    //     std::vector<Shape*> leftResult = query(origin, direction, node->left);
    //     result.insert(result.end(), leftResult.begin(), leftResult.end());
    // }

    // if (node->right) {
    //     std::vector<Shape*> rightResult = query(origin, direction, node->right);
    //     result.insert(result.end(), rightResult.begin(), rightResult.end());
    // }

    // return result;
}
