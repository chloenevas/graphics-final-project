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

void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {

    Camera camera = scene.getCamera();
    glm::vec4 eyePointWorld = glm::inverse(camera.getViewMatrix()) * glm::vec4(0, 0, 0, 1.0f);
    glm::vec3 eyePoint = glm::vec3(eyePointWorld);

    std::vector<Shape*> shapes = makeShapes(scene.getShapes());
    BoundingBox parentBox(glm::vec3(-20.0f, -20.0f, -20.0f), glm::vec3(20.0f, 20.0f, 20.0f));
    KdTree::KdNode* root = kdTree.build(shapes, parentBox);

    bool lens = (scene.getLensInterfaces().size() != 0);

    // arbitrary depth value, can change
    int maxDepth = 3;

    for (int r = 0; r < scene.height(); r ++) {
        for (int c = 0; c < scene.width(); c ++) {
            glm::vec3 d, p;

            if (lens) {
                d = glm::normalize(scene.getPoint(r, c, camera));
                glm::vec3 eyePointLens;
                glm::vec3 dLens;
                if (r == 300 && c == 200) {
                    auto gdg = 0;
                }
                if (!traceRayThroughLens(glm::vec3(0.0f), d, &eyePointLens, &dLens, scene.getLensInterfaces())) {
                    imageData[r * scene.width() + c] = RGBA{0, 0, 0, 255};
                } else {
                    d = glm::normalize(camera.getInverseViewMatrix() * glm::vec4(dLens, 0.0f));
                    p = camera.getInverseViewMatrix() * glm::vec4(eyePointLens, 1.0f);
                    imageData[r * scene.width() + c] = traceRay(r, c, scene, root, p, d, 0);
                }
            } else {
                d = glm::normalize(camera.getInverseViewMatrix() * glm::vec4(scene.getPoint(r, c, camera), 1.0f) - glm::vec4(eyePoint, 1.0f));
                p = eyePoint;
                imageData[r * scene.width() + c] = traceRay(r, c, scene, root, p, d, 0);
            }
        }
    }
}


RGBA RayTracer::traceRay(float r, float c, const RayTraceScene &scene, KdTree::KdNode* root,
                        const glm::vec3 eyePoint, const glm::vec3 d, int currentDepth) {

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
            RGBA reflectionColor = traceRay(r, c, scene, root, offsetIntersection, reflectionDir, currentDepth + 1);

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
            RGBA refractionColor = traceRay(r, c, scene, root, refOffset, T, currentDepth + 1);

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


float RayTracer::calculateColorVariance(RGBA samples[4]) {
    float variance = 0.0f;
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            float dr = static_cast<float>(samples[i].r) - static_cast<float>(samples[j].r);
            float dg = static_cast<float>(samples[i].g) - static_cast<float>(samples[j].g);
            float db = static_cast<float>(samples[i].b) - static_cast<float>(samples[j].b);
            variance += dr * dr + dg * dg + db * db;
        }
    }
    return variance / 6.0f;
}

RGBA RayTracer::averageColor(RGBA samples[4]) {
    float r = 0, g = 0, b = 0;
    for (int i = 0; i < 4; i++) {
        r += static_cast<float>(samples[i].r);
        g += static_cast<float>(samples[i].g);
        b += static_cast<float>(samples[i].b);
    }
    return RGBA{
        static_cast<std::uint8_t>(r / 4),
        static_cast<std::uint8_t>(g / 4),
        static_cast<std::uint8_t>(b / 4),
        255
    };
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
            Sphere sphere = Sphere(translation, SceneMaterial{}, nullptr);
            sphere.setIsLens(true);
            sphere.setRadius(r);
            if (!sphere.calcIntersection(eyePointLens, dLens, intersectionPoint, t)) {
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



