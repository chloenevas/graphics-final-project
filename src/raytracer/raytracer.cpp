
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
        const glm::vec3 velocity = object.primitive.velocity;

        Image* image;
        if (material.textureMap.isUsed){
            image = loadImageFromFile(material.textureMap.filename);
        }
        else{
            image = nullptr;
        }

        switch (object.primitive.type) {
        case PrimitiveType::PRIMITIVE_SPHERE:
            shapes.push_back(new Sphere(ctm, material, velocity, image));
            break;
        case PrimitiveType::PRIMITIVE_CUBE:
            shapes.push_back(new Cube(ctm, material, velocity, image));
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            shapes.push_back(new Cone(ctm, material, velocity, image));
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            shapes.push_back(new Cylinder(ctm, material, velocity, image));
            break;
        default:
            break;
        }
    }

    return shapes;
}

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

    bool lens = (scene.getLensInterfaces().size() != 0);

    for (int r = 0; r < imageHeight; r ++) {
        for (int c = 0; c < imageWidth; c ++) {
            glm::vec4 color(0,0,0,255);
            if (m_config.enableDepthOfField) {
                // Number of samples per pixel
                int samples = 6;  // Increased for better quality

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

                    // dummy unused value for time
                    color += traceRay(scene, root, rayOrigin, finalRayDirection, maxDepth, 0);
                }
                color /= static_cast<float>(samples);

                color = glm::clamp(color, 0.0f, 1.0f);
            } else if (m_config.enableMotionBlur) {
                glm::vec3 d = glm::normalize(camera.getInverseViewMatrix() *
                                                 glm::vec4(scene.getPoint(r, c, camera), 1.0f) - glm::vec4(eyePoint, 1.0f));
                int samples = 8;
                for (int s = 0; s < samples; ++s) {

                    // get a random time within the shutter open and close - start at t = 0 end at t = 1
                    float time = (s + static_cast<float>(rand()) / RAND_MAX) / samples;

                    color += traceRay(scene, root, eyePoint, d, maxDepth, time);
                }
                color /= static_cast<float>(samples);

                color = glm::clamp(color, 0.0f, 1.0f);
            } else if (lens) {
                glm::vec3 d = glm::normalize(scene.getPoint(r, c, camera));
                glm::vec3 eyePointLens;
                glm::vec3 dLens;
                if (!traceRayThroughLens(glm::vec3(0.0f), d, &eyePointLens, &dLens, scene.getLensInterfaces())) {
                    color = glm::vec4(0, 0, 0, 255);
                } else {
                    d = glm::normalize(camera.getInverseViewMatrix() * glm::vec4(dLens, 0.0f));
                    color = traceRay(scene, root, camera.getInverseViewMatrix() * glm::vec4(eyePointLens, 1.0f), d, maxDepth, 0);
                }
            }
            else {

                glm::vec3 d = glm::normalize(camera.getInverseViewMatrix() *
                                                 glm::vec4(scene.getPoint(r, c, camera), 1.0f) - glm::vec4(eyePoint, 1.0f));

                // dummy unused value for time
                color = traceRay(scene, root, eyePoint, d, maxDepth, 0);

            }
            RGBA finalColor;
            finalColor.r = static_cast<std::uint8_t>(color.r * 255.0f);
            finalColor.g = static_cast<std::uint8_t>(color.g * 255.0f);
            finalColor.b = static_cast<std::uint8_t>(color.b * 255.0f);
            finalColor.a = 255;

            imageData[r * imageWidth + c] = finalColor;
        }

    }
}

glm::vec4 RayTracer::traceRay(const RayTraceScene &scene, KdTree::KdNode* root, const glm::vec3 eyePoint, const glm::vec3 d, int currentDepth, float time) {

    const Camera& camera = scene.getCamera();

    float closestT = std::numeric_limits<float>::max();
    glm::vec3 closestIntersection;
    Shape* closestShape = nullptr;

    float velocity = scene.getGlobalData().globalVel;

    std::vector<Shape*> shapes = kdTree.query(eyePoint, d, root);

    for (const auto shape : shapes) {
        float t;
        glm::vec3 intersectionPoint;

        if (shape->calcIntersection(eyePoint, d, intersectionPoint, t, time, velocity)) {
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
            if (light.type == LightType::LIGHT_AREA) {
                // Sample multiple points on the area light for soft shadows
                const int shadowSamples = 8; // Can be adjusted
                float shadowFactor = 0.0f;

                glm::vec3 lightNormal = glm::normalize(glm::vec3(light.dir));
                glm::vec3 lightU = glm::normalize(glm::cross(lightNormal, glm::vec3(0, 1, 0)));
                if (glm::length(lightU) < 0.1f) {
                    lightU = glm::normalize(glm::cross(lightNormal, glm::vec3(1, 0, 0)));
                }
                glm::vec3 lightV = glm::cross(lightNormal, lightU);

                for (int s = 0; s < shadowSamples; s++) {
                    float u = (static_cast<float>(rand()) / RAND_MAX - 0.5f);
                    float v = (static_cast<float>(rand()) / RAND_MAX - 0.5f);

                    glm::vec3 samplePos = glm::vec3(light.pos) +
                                          (u * light.width * lightU) +
                                          (v * light.height * lightV);

                    glm::vec3 shadowDir = glm::normalize(samplePos - offsetIntersection);
                    float maxDist = glm::length(samplePos - offsetIntersection);

                    bool sampleInShadow = false;
                    for (const auto shadowShape : shapes) {
                        float shadowT;
                        glm::vec3 shadowIntersection;

                        if (shadowShape->calcIntersection(offsetIntersection, shadowDir,
                                                          shadowIntersection, shadowT, 0, velocity)) {
                            float shadowDist = glm::length(shadowIntersection - offsetIntersection);
                            if (shadowDist < maxDist) {
                                sampleInShadow = true;
                                break;
                            }
                        }
                    }
                    if (!sampleInShadow) {
                        shadowFactor += 1.0f;
                    }
                }
                shadowFactor /= static_cast<float>(shadowSamples);

                if (shadowFactor > 0.0f) {
                    // std::cout << "Shadow factor: " << shadowFactor << std::endl;
                    glm::vec4 lightContribution = phong(scene, closestIntersection, normal,
                                                        directionToCamera, closestShape->getMaterial(),
                                                        light, closestShape->getTexture(closestIntersection));
                    // std::cout << "Light contribution: " << lightContribution.x << ", " << lightContribution.y << ", " << lightContribution.z << std::endl;
                    illumination += lightContribution * shadowFactor;
                    // std::cout << "Accumulated illumination: " << illumination.x << ", " << illumination.y << ", " << illumination.z << std::endl;
                }
            }else{
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

                    // calculate shadows based on the shape's position at time = 0
                    if (shadowShape->calcIntersection(offsetIntersection, lightDirection, shadowIntersection, shadowT, 0, velocity)) {
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
        }

        glm::vec4 reflectivity = closestShape->getMaterial().cReflective;
        if (reflectivity.r > 0.0f || reflectivity.g > 0.0f || reflectivity.b > 0.0f) {
            if (currentDepth < 4){
                glm::vec3 reflectionDir = glm::reflect(d, normal);
                glm::vec4 reflectionColor = traceRay(scene, root, offsetIntersection, reflectionDir, currentDepth + 1, time);

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
            glm::vec4 refractionColor = traceRay(scene, root, refOffset, T, currentDepth + 1, time);

            illumination.r = glm::mix(illumination.r, refractionColor.r / 255.0f, transparency.r * scene.getGlobalData().kt);
            illumination.g = glm::mix(illumination.g, refractionColor.g / 255.0f, transparency.g * scene.getGlobalData().kt);
            illumination.b = glm::mix(illumination.b, refractionColor.b / 255.0f, transparency.b * scene.getGlobalData().kt);
        }

        illumination = glm::clamp(illumination, 0.0f, 1.0f);
        return illumination;
    } else {
        return glm::vec4(0,0,0,1.0f);
    }
}

bool RayTracer::traceRayThroughLens(const glm::vec3 eyePoint, const glm::vec3 d, glm::vec3 *eyePointOut, glm::vec3 *dOut, std::vector<LensInterface> lenses) {
    glm::vec3 dLens = d;
    glm::vec3 eyePointLens = eyePoint;
    float z = 0.0f;
    for (int i = 0; i < lenses.size(); i++) {
        LensInterface lens = lenses[i];
        z -= lens.thickness;
        float t;
        glm::vec3 n;
        glm::vec3 intersectionPoint;
        bool isStop = (lens.radius == 0);
        if (isStop) {
            t = (z - eyePointLens[2]) / dLens[2];
            intersectionPoint = eyePointLens + t * dLens;
        } else {
            float r = lens.radius;
            float center = z + r;
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, center));
            Sphere sphere = Sphere(translation, SceneMaterial{}, glm::vec3(0.0), nullptr);
            sphere.setIsLens(true);
            sphere.setRadius(r);
            if (!sphere.calcIntersection(eyePointLens, dLens, intersectionPoint, t, 0.0, 0.0)) {
                return false;
            } else {
                n = sphere.calcNormal(intersectionPoint);
            }
            float n1 = lens.n;
            float n2 = (i < lenses.size() - 1 && lenses[i + 1].n != 0) ? lenses[i+1].n : 1.0f;
            glm::vec3 outputD;
            if (!refract(glm::normalize(-dLens), n, n1, n2, &outputD)) {
                return false;
            }
            dLens = outputD;
        }
        float l = std::sqrt(intersectionPoint[0] * intersectionPoint[0] + intersectionPoint[1] * intersectionPoint[1]);
        if (l > lens.aperture) {
            return false;
        }
        eyePointLens = intersectionPoint;
    }
    *dOut = dLens;
    *eyePointOut = eyePointLens;
    return true;
}

bool RayTracer::refract(glm::vec3 d, glm::vec3 normal, float n1, float n2, glm::vec3 *outputD) {
    float eta = n1/n2;
    float cos1 = glm::dot(d, normal);
    bool entering = cos1 < 0.0f;
    cos1 = entering ? -cos1 : cos1;
    float sin1 = std::max(0.0f, 1.0f - cos1 * cos1);
    float sin2 = eta * eta * sin1;
    if (sin2 >= 1.0f) {
        return false;
    }
    float cos2 = std::sqrt(1.0f - sin2);
    *outputD = glm::normalize(eta * -d + (eta * cos1 - cos2) * normal);

    return true;
}
