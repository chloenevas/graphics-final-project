#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

void traverseSceneGraph(SceneNode* node, glm::mat4 parentCTM, std::vector<RenderShapeData> &shapes, std::vector<SceneLightData> &lights) {
    glm::mat4 currentCTM = parentCTM;

    for (const auto& transformation: node->transformations) {
        switch (transformation->type) {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            currentCTM = currentCTM * glm::translate(glm::mat4(1.0f), transformation->translate);
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            currentCTM = currentCTM * glm::scale(glm::mat4(1.0f), transformation->scale);
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            currentCTM = currentCTM * glm::rotate(glm::mat4(1.0f), transformation->angle, transformation->rotate);
            break;
        case TransformationType::TRANSFORMATION_MATRIX:
            currentCTM = currentCTM * transformation->matrix;
            break;
        default:
            break;
        }
    }

    for (const auto& primitive: node->primitives) {
        RenderShapeData shapeData;
        shapeData.primitive = *primitive;
        shapeData.ctm = currentCTM;
        shapes.push_back(shapeData);
    }

    for (const auto& light : node->lights) {
        SceneLightData lightData;
        lightData.id = light->id;
        lightData.type = light->type;
        lightData.color = light->color;
        lightData.function = light->function;

        switch (light->type) {
        case LightType::LIGHT_POINT:
            lightData.pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            lightData.pos = currentCTM * lightData.pos;
            lightData.dir = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            lightData.penumbra = 0.0f;
            lightData.angle = 0.0f;
            break;

        case LightType::LIGHT_DIRECTIONAL:
            lightData.pos = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            lightData.dir = light->dir;
            lightData.dir = glm::normalize(currentCTM * lightData.dir);
            lightData.penumbra = 0.0f;
            lightData.angle = 0.0f;
            break;

        case LightType::LIGHT_SPOT:
            lightData.pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            lightData.pos = currentCTM * lightData.pos;
            lightData.dir = light->dir;
            lightData.dir = glm::normalize(currentCTM * lightData.dir);
            lightData.penumbra = light->penumbra;
            lightData.angle = light->angle;
            break;

        case LightType::LIGHT_AREA:
            std::cout << "Hello, World!" << std::endl;
            lightData.pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            lightData.pos = currentCTM * lightData.pos;  // Transform the position
            lightData.dir = light->dir;
            lightData.dir = glm::normalize(currentCTM * lightData.dir);  // Transform the direction
            lightData.width = light->width;
            lightData.height = light->height;  // Keep original width/height
            lightData.function = light->function;
            break;

        default:
            break;
        }

        lights.push_back(lightData);
    }

    for (SceneNode* child: node->children) {
        traverseSceneGraph(child, currentCTM, shapes, lights);
    }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();
    renderData.shapes.clear();

    SceneNode* rootNode = fileReader.getRootNode();

    glm::mat4 identityMatrix = glm::mat4(1.0f);
    traverseSceneGraph(rootNode, identityMatrix, renderData.shapes, renderData.lights);
    return true;
}
