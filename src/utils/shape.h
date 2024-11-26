#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include "scenedata.h"
#include "boundingbox.h"
#include "imagereader.h"

class Shape {
public:
    virtual ~Shape() = default;

    Shape(const glm::mat4& ctm, const SceneMaterial& material, const Image* image)
        : m_material(material), m_ctm(ctm), m_image(image) {
        m_inverseCTM = glm::inverse(m_ctm);
    }

     const SceneMaterial& getMaterial() const {
         return m_material;
     }

     // const glm::mat4& getCTM() const {
     //     return m_ctm;
     // }

     // const int getTextHeight() const {
     //     return m_image->height;
     // }

     // const int getTextWidth() const {
     //     return m_image->width;
     // }

    virtual glm::vec3 calcNormal(const glm::vec3 point) = 0;

    virtual bool calcIntersection(const glm::vec3 rayOrigin, const glm::vec3 rayDirection, glm::vec3& intersectionPoint, float &t) = 0;

    virtual BoundingBox getBoundingBox() = 0;

    virtual double surfaceArea() = 0;

    virtual glm::vec3 getTexture(const glm::vec3& intersection) = 0;

    virtual void id() = 0;

protected:
    SceneMaterial m_material;
    glm::mat4 m_ctm;
    glm::mat4 m_inverseCTM;
    const Image* m_image;
};

#endif // SHAPE_H
