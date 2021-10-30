#include "cFirework.h"

cWorldSpace* cFirework::_world = cWorldSpace::Instance();

bool cFirework::isInTheAir()
{
    return GetPosition().y > 0.f;
}

