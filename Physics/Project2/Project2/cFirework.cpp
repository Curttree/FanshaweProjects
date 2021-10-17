#include "cFirework.h"

cWorldSpace* cFirework::_world = cWorldSpace::Instance();
void cFirework::update() {
    if (glm::dot(GetVelocity(), _world->axes[1]) <= 0){
        hitApex = true;
    }
}

bool cFirework::isInTheAir()
{
    return GetPosition().y > 0.f; // TODO: Determine if radius of particle should factor in.
}

