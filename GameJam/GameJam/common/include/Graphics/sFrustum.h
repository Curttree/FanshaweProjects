#pragma once
#include <glm/ext/vector_float3.hpp>

struct sPlane
{
    sPlane() {};
    sPlane(const glm::vec3& _point, const glm::vec3& _normal)
        : normal(glm::normalize(_normal)),
        distance(glm::dot(_normal, _point))
    {}
    float distance = 0.f;
    glm::vec3 normal = { 0.f, 1.f, 0.f };
};

struct sFrustum
{
    sPlane topFace;
    sPlane bottomFace;

    sPlane rightFace;
    sPlane leftFace;

    sPlane farFace;
    sPlane nearFace;
};