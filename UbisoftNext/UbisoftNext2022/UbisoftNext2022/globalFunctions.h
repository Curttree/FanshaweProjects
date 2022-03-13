#pragma once

#include <stdlib.h>
#include "sVec2.h"

template <class T>
T gGetRandBetween(T LO, T HI)
{
    float fLO = static_cast<float>(LO);
    float fHI = static_cast<float>(HI);
    float r3 = fLO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (fHI - fLO)));
    return r3;
}

float gComparePositions(Vec2 pos1, Vec2 pos2) {
    Vec2 resultVector;
    resultVector.x = pos2.x - pos1.x;
    resultVector.y = pos2.y - pos1.y;
    return sqrt(pow(resultVector.x, 2) + pow(resultVector.y, 2));
}