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