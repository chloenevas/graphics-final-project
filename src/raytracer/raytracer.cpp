#include "raytracer.h"
#include "raytracescene.h"
#include <glm/glm.hpp>
#include "utils/shape.h"
#include "utils/sphere.h"
#include "utils/cube.h"
#include "utils/cone.h"
#include "utils/cylinder.h"
#include "utils/lightmodel.h"
#include "utils/imagereader.h"
#include <iostream>

RayTracer::RayTracer(Config config) :
    m_config(config)
{}

// Helper function to convert illumination to RGBA, applying some form of tone-mapping (e.g. clamping) in the process
RGBA toRGBA(const glm::vec4 &illumination) {
    unsigned char r = static_cast<unsigned char>(255 * glm::clamp(illumination.r, 0.0f, 1.0f));
    unsigned char g = static_cast<unsigned char>(255 * glm::clamp(illumination.g, 0.0f, 1.0f));
    unsigned char b = static_cast<unsigned char>(255 * glm::clamp(illumination.b, 0.0f, 1.0f));

    return RGBA{r, g, b};
}

// Function to create the appropriate shape based on the primitive type and apply the CTM
std::vector<Shape*> RayTracer::makeShapes(const std::vector<RenderShapeData>& shapeData) {
    std::vector<Shape*> shapes;

    for (const auto& object : shapeData) {
        const glm::mat4& ctm = object.ctm;
        const SceneMaterial& material = object.primitive.material;
        Image* image;
        if (material.textureMap.isUsed){
            image = loadImageFromFile(material.textureMap.filename);
        }
        else{
            image = nullptr;
        }

        switch (object.primitive.type) {
        case PrimitiveType::PRIMITIVE_SPHERE:
            shapes.push_back(new Sphere(ctm, material, image));
            break;
        case PrimitiveType::PRIMITIVE_CUBE:
            shapes.push_back(new Cube(ctm, material, image));
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            shapes.push_back(new Cone(ctm, material, image));
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            shapes.push_back(new Cylinder(ctm, material, image));
            break;
        default:
            break;
        }
    }

    return shapes;
}

// Previous implementation without super sampling

// void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {

//     Camera camera = scene.getCamera();

//     glm::vec4 eyePointWorld = glm::inverse(camera.getViewMatrix()) * glm::vec4(0, 0, 0, 1.0f);
//     glm::vec3 eyePoint = glm::vec3(eyePointWorld);

//     for (int r = 0; r < scene.height(); r++) {
//         for (int c = 0; c < scene.width(); c++) {
//             glm::vec3 d = glm::normalize(camera.getInverseViewMatrix() * glm::vec4(scene.getPoint(r, c, camera), 1.0f) - eyePointWorld);

//             float closestT = std::numeric_limits<float>::max();
//             glm::vec3 closestIntersection;
//             Shape* closestShape = nullptr;

//             std::vector<Shape*> shapes = makeShapes(scene.getShapes());

//             for (const auto &shape : shapes) {
//                 float t;
//                 glm::vec3 intersectionPoint;

//                 if (shape->calcIntersection(eyePoint, d, intersectionPoint, t)) {

//                     float worldT = glm::length(intersectionPoint - eyePoint);

//                     if (worldT < closestT) {
//                         closestT = worldT;
//                         closestIntersection = intersectionPoint;
//                         closestShape = shape;
//                     }
//                 }
//             }

//             if (closestShape != nullptr) {
//                  glm::vec3 normal = closestShape->calcNormal(closestIntersection);
//                  glm::vec3 directionToCamera = glm::normalize(eyePoint - closestIntersection);
//                  RGBA color = phong(scene, closestIntersection, normal, directionToCamera, closestShape->getMaterial());
//                  imageData[r * scene.width() + c] = color;
//             }
//         }
//     }
// }

void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {

    Camera camera = scene.getCamera();
    glm::vec4 eyePointWorld = glm::inverse(camera.getViewMatrix()) * glm::vec4(0, 0, 0, 1.0f);
    glm::vec3 eyePoint = glm::vec3(eyePointWorld);

    std::vector<Shape*> shapes = makeShapes(scene.getShapes());
    BoundingBox parentBox(glm::vec3(-20.0f, -20.0f, -20.0f), glm::vec3(20.0f, 20.0f, 20.0f));
    KdTree::KdNode* root = kdTree.build(shapes, parentBox);

    // arbitrary depth value, can change
    int maxDepth = 3;

    int imageWidth = scene.width();
    int imageHeight = scene.height();

    for (int r = 0; r < imageHeight; r ++) {
        for (int c = 0; c < imageWidth; c ++) {
            RGBA color = {0,0,0,255};
            if (m_config.enableDepthOfField) {
                // Use floating point accumulation for colors
                float accumR = 0.0f;
                float accumG = 0.0f;
                float accumB = 0.0f;

                // Number of samples per pixel
                int samples = 16;  // Increased for better quality

                for (int s = 0; s < samples; ++s) {
                    float aspectRatio = camera.getAspectRatio();

                    // Add small random offset to pixel coordinates for anti-aliasing
                    float jitterX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.5f;
                    float jitterY = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.5f;

                    float imageSpaceCoordX = (static_cast<float>(c) + 0.5f + jitterX) / static_cast<float>(imageWidth);
                    float imageSpaceCoordY = (static_cast<float>(r) + 0.5f + jitterY) / static_cast<float>(imageHeight);

                    float multiplierX = 2.0f * imageSpaceCoordX - 1.0f;
                    float multiplierY = 1.0f - 2.0f * imageSpaceCoordY;

                    float viewplaneHeight = 2.0f * tan(camera.getHeightAngle() / 2.0f);
                    float viewplaneWidth = aspectRatio * viewplaneHeight;

                    glm::vec3 cameraPos = glm::vec3(camera.getPosition());
                    glm::vec3 cameraLook = glm::normalize(glm::vec3(camera.getLook()));
                    glm::vec3 cameraUpInitial = glm::normalize(glm::vec3(camera.getUp()));
                    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraLook, cameraUpInitial));
                    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraLook));

                    glm::vec3 viewplaneCenter = cameraPos + cameraLook;
                    glm::vec3 horizontal = (viewplaneWidth / 2.0f) * cameraRight;
                    glm::vec3 vertical = (viewplaneHeight / 2.0f) * cameraUp;
                    glm::vec3 viewplanePoint = viewplaneCenter + (multiplierX * horizontal) + (multiplierY * vertical);

                    glm::vec3 rayDirection = glm::normalize(viewplanePoint - cameraPos);
                    glm::vec3 focalPoint = cameraPos + camera.getFocalLength() * rayDirection;

                    glm::vec3 randomDiskPoint = camera.random_in_unit_disk();
                    glm::vec3 offset = (camera.getAperture() / 2.0f) *
                                       (randomDiskPoint.x * cameraRight + randomDiskPoint.y * cameraUp);

                    glm::vec3 rayOrigin = cameraPos + offset;
                    glm::vec3 finalRayDirection = glm::normalize(focalPoint - rayOrigin);

                    RGBA tempColor = traceRay(scene, root, rayOrigin, finalRayDirection, maxDepth);

                    // Accumulate colors as floats
                    accumR += static_cast<float>(tempColor.r);
                    accumG += static_cast<float>(tempColor.g);
                    accumB += static_cast<float>(tempColor.b);
                }

                // Average and clamp the accumulated colors
                RGBA finalColor;
                finalColor.r = static_cast<uint8_t>(std::clamp(accumR / samples, 0.0f, 255.0f));
                finalColor.g = static_cast<uint8_t>(std::clamp(accumG / samples, 0.0f, 255.0f));
                finalColor.b = static_cast<uint8_t>(std::clamp(accumB / samples, 0.0f, 255.0f));
                finalColor.a = 255;

                imageData[r * imageWidth + c] = finalColor;
            } else {
                glm::vec3 d = glm::normalize(camera.getInverseViewMatrix() *
                                                 glm::vec4(scene.getPoint(r, c, camera), 1.0f) - glm::vec4(eyePoint, 1.0f));
                imageData[r * imageWidth + c] = traceRay(scene, root, eyePoint, d, maxDepth);
            }
        }
    }
}

RGBA RayTracer::traceRay(const RayTraceScene &scene, KdTree::KdNode* root, const glm::vec3 eyePoint, const glm::vec3 d, int currentDepth) {

    const Camera& camera = scene.getCamera();

    // glm::vec3 rayOrigin;
    // glm::vec3 rayDirection;

    // if (camera.getAperture() > 0.0f) {
    //     // Calculate the focal point first
    //     glm::vec3 focalPoint = eyePoint + d * camera.getFocalLength();

    //     // Get the new ray origin with defocus blur
    //     rayOrigin = camera.getRayOrigin();

    //     // The ray direction is from the new origin to the focal point
    //     rayDirection = glm::normalize(focalPoint - rayOrigin);
    // } else {
    //     // If no depth of field (aperture = 0), use original ray parameters
    //     rayOrigin = eyePoint;
    //     rayDirection = d;
    // }

    float closestT = std::numeric_limits<float>::max();
    glm::vec3 closestIntersection;
    Shape* closestShape = nullptr;

    std::vector<Shape*> shapes = kdTree.query(eyePoint, d, root);

    for (const auto shape : shapes) {
        float t;
        glm::vec3 intersectionPoint;

        if (shape->calcIntersection(eyePoint, d, intersectionPoint, t)) {
            float worldT = glm::length(intersectionPoint - eyePoint);

            if (worldT < closestT) {
                closestT = worldT;
                closestIntersection = intersectionPoint;
                closestShape = shape;
            }
        }
    }

    if (closestShape != nullptr) {
        glm::vec3 normal = closestShape->calcNormal(closestIntersection);
        const float epsilon = 1e-2f;
        glm::vec3 offsetIntersection = closestIntersection + epsilon * normal;
        glm::vec3 directionToCamera = glm::normalize(-d);

        glm::vec3 ambient = scene.getGlobalData().ka * closestShape->getMaterial().cAmbient;
        glm::vec4 illumination = glm::vec4(ambient, 1.0f);

        for (const SceneLightData &light : scene.getLights()) {
            glm::vec3 lightDirection;
            float maxDistance = std::numeric_limits<float>::max();

            if (light.type == LightType::LIGHT_POINT || light.type == LightType::LIGHT_SPOT) {
                lightDirection = glm::normalize(glm::vec3(light.pos) - offsetIntersection);
                maxDistance = glm::length(glm::vec3(light.pos) - offsetIntersection);
            } else {
                lightDirection = glm::normalize(glm::vec3(-light.dir));
            }

            bool isInShadow = false;

            for (const auto shadowShape : shapes) {
                float shadowT;
                glm::vec3 shadowIntersection;

                if (shadowShape->calcIntersection(offsetIntersection, lightDirection, shadowIntersection, shadowT)) {
                    float shadowDistance = glm::length(shadowIntersection - offsetIntersection);

                    if (shadowDistance < maxDistance || light.type == LightType::LIGHT_DIRECTIONAL) {
                        isInShadow = true;
                        break;
                    }
                }
            }

            if (!isInShadow) {
                glm::vec4 lightContribution = phong(scene, closestIntersection, normal, directionToCamera,
                                                    closestShape->getMaterial(), light, closestShape->getTexture(closestIntersection));
                illumination += lightContribution;
            }
        }

        glm::vec4 reflectivity = closestShape->getMaterial().cReflective;
        if (reflectivity.r > 0.0f || reflectivity.g > 0.0f || reflectivity.b > 0.0f) {
            if (currentDepth < 4){
            glm::vec3 reflectionDir = glm::reflect(d, normal);
            RGBA reflectionColor = traceRay(scene, root, offsetIntersection, reflectionDir, currentDepth + 1);

            illumination += glm::vec4(
                scene.getGlobalData().ks * reflectivity.r * (reflectionColor.r / 255.0f),
                scene.getGlobalData().ks * reflectivity.g * (reflectionColor.g / 255.0f),
                scene.getGlobalData().ks * reflectivity.b * (reflectionColor.b / 255.0f),
                1.0f
                );
        }
    }
        glm::vec4 transparency = closestShape->getMaterial().cTransparent;
        float ior = closestShape->getMaterial().ior;
        if ((transparency.r > 0.0f || transparency.g > 0.0f || transparency.b > 0.0f) && currentDepth < 4) {
            float cosTheta1 = glm::dot(d, normal);
            bool entering = cosTheta1 < 0.0f;
            cosTheta1 = entering ? -cosTheta1 : cosTheta1;
            float eta = entering ? 1.0f / ior : ior;
            glm::vec3 refNorm = entering ? normal : -normal;


            float k = 1.0f - eta * eta * (1.0f - cosTheta1 * cosTheta1);

            glm::vec3 T;
            if (k < 0.0f) {
                T = d - 2.0f * glm::dot(d, refNorm) * refNorm;
            } else {
                float cosTheta2 = std::sqrt(k);
                T = (eta * d) + (((eta * cosTheta1) - cosTheta2) * refNorm);
            }

            glm::vec3 refOffset = closestIntersection + epsilon * T;
            RGBA refractionColor = traceRay(scene, root, refOffset, T, currentDepth + 1);

            illumination.r = glm::mix(illumination.r, refractionColor.r / 255.0f, transparency.r * scene.getGlobalData().kt);
            illumination.g = glm::mix(illumination.g, refractionColor.g / 255.0f, transparency.g * scene.getGlobalData().kt);
            illumination.b = glm::mix(illumination.b, refractionColor.b / 255.0f, transparency.b * scene.getGlobalData().kt);
        }

        illumination = glm::clamp(illumination, 0.0f, 1.0f);
        return toRGBA(illumination);
    } else {
        return RGBA{0, 0, 0, 255};
    }
}

