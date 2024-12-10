#pragma once

#include <glm/glm.hpp>
#include "utils/rgba.h"
#include "utils/shape.h"
#include "raytracescene.h"
#include "kdtree.h"

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
        bool enableDepthOfField  = false;
        int maxRecursiveDepth    = 4;
        bool onlyRenderNormals   = false;
    };

public:
    RayTracer(Config config);

    std::vector<Shape*> makeShapes(const std::vector<RenderShapeData>& shapeData);

    // Renders the scene synchronously.
    // The ray-tracer will render the scene and fill imageData in-place.
    // @param imageData The pointer to the imageData to be filled.
    // @param scene The scene to be rendered.
    void render(RGBA *imageData, const RayTraceScene &scene);

    RGBA superSamp(float r, float c, int pixelSize, const RayTraceScene &scene, KdTree::KdNode* root,
                             const Camera &camera, const glm::vec3 &eyePoint, int maxDepth, bool lens);

    RGBA traceRay(float r, float c, const RayTraceScene &scene, KdTree::KdNode* root, const glm::vec3 eyePoint, const glm::vec3 d, int currentDepth);

    float calculateColorVariance(RGBA samples[4]);

    RGBA averageColor(RGBA samples[4]);

    bool traceRayThroughLens(const glm::vec3 eyePoint, const glm::vec3 d, glm::vec3 *eyePointOut, glm::vec3 *dOut, std::vector<LensInterface> lenses);

    bool refract(glm::vec3 d, glm::vec3 normal, float n1, float n2, glm::vec3 *outputD);

private:
    const Config m_config;
    KdTree kdTree;
};

