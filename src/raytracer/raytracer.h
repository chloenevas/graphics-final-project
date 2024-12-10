
#pragma once

#include <glm/glm.hpp>
#include "utils/rgba.h"
#include "utils/shape.h"
#include "raytracescene.h"
#include "kdtree.h"
#include <random>

// A forward declaration for the RaytraceScene class

class RayTraceScene;

// A class representing a ray-tracer

class RayTracer
{
public:
    struct Config {
        bool enableShadow        = false;
        bool enableReflection    = false;
        bool enableRefraction    = false;
        bool enableTextureMap    = false;
        bool enableTextureFilter = false;
        bool enableParallelism   = false;
        bool enableSuperSample   = false;
        bool enableAcceleration  = false;
        bool enableDepthOfField  = true;
        int maxRecursiveDepth    = 4;
        bool onlyRenderNormals   = false;
        bool samples_per_pixel = 100;
    };

public:
    RayTracer(Config config);

    std::vector<Shape*> makeShapes(const std::vector<RenderShapeData>& shapeData);

    // Renders the scene synchronously.
    // The ray-tracer will render the scene and fill imageData in-place.
    // @param imageData The pointer to the imageData to be filled.
    // @param scene The scene to be rendered.
    void render(RGBA *imageData, const RayTraceScene &scene);

    glm::vec4 traceRay(const RayTraceScene &scene, KdTree::KdNode* root, const glm::vec3 eyePoint, const glm::vec3 d, int currentDepth);


private:
    const Config m_config;
    KdTree kdTree;
};
