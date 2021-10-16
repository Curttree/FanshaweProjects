#include "cFirework.h"

cWorldSpace* cFirework::_world = cWorldSpace::Instance();
bool cFirework::isGoingUp() {
    return glm::dot(GetVelocity(), _world->axes[1]) > 0;
}

bool cFirework::isInTheAir()
{
    return GetPosition().y > 0.f; // TODO: Determine if radius of particle should factor in.
}

bool cFirework::isReadyForStageTwo() {
    if (!isGoingUp()) {
        //std::cout << "Bang" << std::endl;
        return true;
    }
    return false;
}