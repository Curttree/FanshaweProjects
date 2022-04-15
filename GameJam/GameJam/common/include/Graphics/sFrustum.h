#pragma once
#include <glm/ext/vector_float3.hpp>

struct sPlane
{
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