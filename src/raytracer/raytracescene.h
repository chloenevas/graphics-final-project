#pragma once

#include "utils/scenedata.h"
#include "utils/sceneparser.h"
#include "camera/camera.h"

// A class representing a scene to be ray-traced

// Feel free to make your own design choices for RayTraceScene, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class RayTraceScene
{
private:
    int m_width;
    int m_height;
    SceneGlobalData m_globalData;
    Camera m_camera;
    std::vector<RenderShapeData> shapes;
    std::vector<SceneLightData> lights;
public:
    RayTraceScene(int width, int height, const RenderData &metaData);

    // The getter of the width of the scene
    const int& width() const;

    // The getter of the height of the scene
    const int& height() const;

    // The getter of the global data of the scene
    const SceneGlobalData& getGlobalData() const;

    // The getter of the shared pointer to the camera instance of the scene
    const Camera& getCamera() const;

    const std::vector<RenderShapeData>& getShapes() const;

    const std::vector<SceneLightData>& getLights() const;

    const glm::vec3 getPoint(float i, float j, const Camera& camera) const;
};
