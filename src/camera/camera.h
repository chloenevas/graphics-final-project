#pragma once

#include <glm/glm.hpp>
#include "utils/sceneparser.h"

// A class representing a virtual camera.

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {
private:
    glm::mat4 m_viewMatrix;  // Stores the view matrix of the camera
    glm::mat4 m_inverseViewMatrix;
    float m_aspectRatio;      // Stores the aspect ratio (width / height)
    float m_heightAngle;      // Stores the height angle in radians
    float m_widthAngle;
    float m_focalLength;      // Stores the focal length for depth of field (optional)
    float m_aperture;

public:
    Camera(int width, int height, const RenderData &metaData);

    glm::mat4 calcViewMatrix(glm::vec3 pos, glm::vec3 look, glm::vec3 up);

    const glm::mat4 getViewMatrix() const;

    const glm::mat4 getInverseViewMatrix() const;
    // Returns the view matrix for the current camera settings.
    // You might also want to define another function that return the inverse of the view matrix.

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    float getWidthAngle() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getFocalLength() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getAperture() const;
};
