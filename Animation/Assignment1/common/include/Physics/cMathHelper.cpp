#include "cMathHelper.h"

cMathHelper::cMathHelper()
{
}

cMathHelper* cMathHelper::_instance = 0;
cMathHelper* cMathHelper::Instance() {
	if (_instance == 0) {
		_instance = new cMathHelper;
	}

	return _instance;
}

float cMathHelper::getRandom()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float cMathHelper::getRandom(float high)
{
    return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / high);
}

float cMathHelper::getRandom(float low, float high)
{
    return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
}
