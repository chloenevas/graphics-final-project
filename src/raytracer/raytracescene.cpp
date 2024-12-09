#include "raytracescene.h"

RayTraceScene::RayTraceScene(int width, int height, const RenderData &metaData)
    : m_width(width), m_height(height), m_globalData(metaData.globalData), m_camera(width, height, metaData),
    shapes(metaData.shapes), lights(metaData.lights), lensInterfaces(metaData.lensInterfaces) {}

// Getter for width
const int& RayTraceScene::width() const {
    return m_width;
}

// Getter for height
const int& RayTraceScene::height() const {
    return m_height;
}

// Getter for global data
const SceneGlobalData& RayTraceScene::getGlobalData() const {
    return m_globalData;
}

// Getter for camera
const Camera& RayTraceScene::getCamera() const {
    return m_camera;
}

// Getter for shapes
const std::vector<RenderShapeData>& RayTraceScene::getShapes() const {
    return shapes;
}

// Getter for lights
const std::vector<SceneLightData>& RayTraceScene::getLights() const {
    return lights;
}

const glm::vec3 RayTraceScene::getPoint(float r, float c, const Camera& camera) const {
    float viewPlaneHeight = 2 * camera.getFocalLength() * tan(camera.getHeightAngle() / 2.0f);
    float viewPlaneWidth = 2 * camera.getFocalLength() * tan(camera.getWidthAngle() / 2.0f);

    float xNorm = (c + 0.5f) / m_width - 0.5f;
    float yNorm = (m_height - 1 - r + 0.5f) / m_height - 0.5f;

    float x = xNorm * viewPlaneWidth;
    float y = yNorm * viewPlaneHeight;
    float z = -camera.getFocalLength();

    return glm::vec3(x, y, z);
}

const std::vector<LensInterface>& RayTraceScene::getLensInterfaces() const {
    return lensInterfaces;
}
