#pragma once

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <glm/glm.hpp>

#include "utils/sceneparser.h"
#include "camera/camera.h"
#include <QLabel>

#include <glm/glm.hpp>
#include "utils/rgba.h"
#include "utils/shape.h"
#include "raytracescene.h"
#include "kdtree.h"


// A forward declaration for the RaytraceScene class

class RayTraceScene;

// A class representing a ray-tracer

class RayTracer : public QWidget
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
                             const Camera &camera, const glm::vec3 &eyePoint, int maxDepth);

    RGBA traceRay(float r, float c, const RayTraceScene &scene, KdTree::KdNode* root, const glm::vec3 eyePoint, const glm::vec3 d, int currentDepth);

    float calculateColorVariance(RGBA samples[4]);

    RGBA averageColor(RGBA samples[4]);

private:
    const Config m_config;
    KdTree kdTree;
};

