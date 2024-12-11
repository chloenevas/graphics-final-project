#include "lightmodel.h"

glm::vec4 phong(const RayTraceScene &scene,
           glm::vec3 position,
           glm::vec3 normal,
           glm::vec3 directionToCamera,
           SceneMaterial material,
           SceneLightData light,
           glm::vec3 texture) {
    glm::vec4 illumination(0, 0, 0, 1);
    if (light.type == LightType::LIGHT_AREA) {
        const int numSamples = 8;
        glm::vec4 totalIllumination(0, 0, 0, 1);

        glm::vec3 lightNormal = glm::normalize(glm::vec3(light.dir));

        glm::vec3 upVector = (std::abs(glm::dot(lightNormal, glm::vec3(0, 1, 0))) > 0.9f) ?
                                 glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);

        glm::vec3 lightU = glm::normalize(glm::cross(lightNormal, upVector));
        glm::vec3 lightV = glm::normalize(glm::cross(lightU, lightNormal));

        for (int i = 0; i < numSamples; i++) {
            float u = light.width * (static_cast<float>(rand()) / RAND_MAX - 0.5f);
            float v = light.height * (static_cast<float>(rand()) / RAND_MAX - 0.5f);

            glm::vec3 samplePos = glm::vec3(light.pos) + (u * lightU) + (v * lightV);

            glm::vec3 directionToLight = samplePos - position;
            float distance = glm::length(directionToLight);
            glm::vec3 normalizedDirToLight = glm::normalize(directionToLight);

            float sampleAttenuation = 1.0f / (light.function.x +
                                              light.function.y * distance +
                                              light.function.z * distance * distance);
            sampleAttenuation = glm::min(sampleAttenuation, 1.0f);

            float dotProduct = glm::dot(normal, normalizedDirToLight);
            if (dotProduct > 0) {
                glm::vec4 diffuseBlend = glm::mix(material.cDiffuse * scene.getGlobalData().kd,
                                                  glm::vec4(texture, 0.0f),
                                                  material.blend);

                totalIllumination += diffuseBlend * dotProduct * light.color * sampleAttenuation;

                glm::vec3 reflection = glm::reflect(normalizedDirToLight, normal);
                float specAngle = glm::dot(glm::normalize(-reflection), directionToCamera);
                if (specAngle > 0) {
                    float specFactor = glm::pow(specAngle, material.shininess);
                    glm::vec3 specular = material.cSpecular * scene.getGlobalData().ks *
                                         specFactor * light.color * sampleAttenuation;
                    totalIllumination += glm::vec4(specular, 0.0f);
                }
            }
        }

        illumination += totalIllumination / float(numSamples);
        return illumination;
    }

    float attenuation = 1.0f;
    glm::vec3 lightDirection;
    float intensity = 1.0f;

    if (light.type == LightType::LIGHT_POINT || light.type == LightType::LIGHT_SPOT) {
        glm::vec3 directionToLight = glm::vec3(light.pos) - position;
        float distance = glm::length(directionToLight);
        lightDirection = glm::normalize(directionToLight);

        attenuation = 1.0f / (light.function.x +
                              light.function.y * distance +
                              light.function.z * distance * distance);

        attenuation = glm::min(attenuation, 1.0f);

        if (light.type == LightType::LIGHT_SPOT) {
            float angle = glm::dot(-lightDirection, glm::normalize(glm::vec3(light.dir)));
            float thetaInner = glm::cos(light.angle - light.penumbra);
            float thetaOuter = glm::cos(light.angle);

            if (angle > thetaOuter) {
                if (angle < thetaInner) {
                    float falloffFactor = -2.0f * glm::pow((angle - thetaInner) / (thetaOuter - thetaInner), 3)
                    + 3.0f * glm::pow((angle - thetaInner) / (thetaOuter - thetaInner), 2);
                    intensity = (1 - falloffFactor);
                }
            }
            else {
                intensity = 0;
            }
        }
    }
    else {
        lightDirection = glm::normalize(glm::vec3(-light.dir));
    }

    float dotProduct = glm::dot(normal, lightDirection);
    dotProduct = dotProduct > 0 ? dotProduct : 0;
    glm::vec4 diffuseBlend = glm::mix(material.cDiffuse * scene.getGlobalData().kd, glm::vec4(texture, 0.0f), material.blend);
    glm::vec4 diffuse = diffuseBlend * dotProduct * intensity * light.color * attenuation;
    illumination += diffuse;

    glm::vec3 reflection = glm::reflect(lightDirection, normal);

    float specAngle = glm::dot(glm::normalize(-reflection), directionToCamera);
    specAngle = specAngle > 0 ? specAngle : 0.0f;
    float specFactor = glm::pow(specAngle, material.shininess);
    glm::vec3 specular = material.cSpecular * scene.getGlobalData().ks * specFactor * intensity *  light.color * attenuation;
    illumination += glm::vec4(specular, 0.0f);

    return illumination;
}

