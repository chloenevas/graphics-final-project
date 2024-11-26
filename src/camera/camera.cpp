#include <stdexcept>
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int width, int height, const RenderData &metaData)
    : m_viewMatrix(Camera::calcViewMatrix(metaData.cameraData.pos, metaData.cameraData.look, metaData.cameraData.up)),
    m_inverseViewMatrix(glm::inverse(m_viewMatrix)),
    m_aspectRatio(static_cast<float>(width) / height),
    m_heightAngle(metaData.cameraData.heightAngle),
    m_widthAngle(m_heightAngle*m_aspectRatio),
    m_focalLength(1.0f),
    m_aperture(metaData.cameraData.aperture) {}

glm::mat4 Camera::calcViewMatrix(glm::vec3 pos, glm::vec3 look, glm::vec3 up) {
    glm::vec3 w = glm::normalize(-look);
    glm::vec3 v = glm::normalize(up - glm::dot(up, w) * w);
    glm::vec3 u = glm::cross(v, w);

    glm::mat4 rotate = {
        u.x, v.x, w.x, 0,
        u.y, v.y, w.y, 0,
        u.z, v.z, w.z, 0,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    glm::mat4 translate = {
        1,0,0, 0,
        0,1,0, 0,
        0,0,1, 0,
        -pos.x, -pos.y, -pos.z, 1.0f
    };

    return rotate * translate;
}

// Getter for view matrix
const glm::mat4 Camera::getViewMatrix() const {
    return m_viewMatrix;
}

const glm::mat4 Camera::getInverseViewMatrix() const {
    return m_inverseViewMatrix;
}

// Getter for aspect ratio
float Camera::getAspectRatio() const {
    return m_aspectRatio;
}

// Getter for height angle (in radians)
float Camera::getHeightAngle() const {
    return m_heightAngle;
}

float Camera::getWidthAngle() const {
    return m_widthAngle;
}

// Getter for focal length (optional, for depth of field)
float Camera::getFocalLength() const {
    return m_focalLength;
}

// Getter for aperture (optional, for depth of field)
float Camera::getAperture() const {
    return m_aperture;
}
