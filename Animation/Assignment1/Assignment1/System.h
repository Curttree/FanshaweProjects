#pragma once

#include <vector>
#include "cEntity.h"
using std::vector;

class System
{
public:
	virtual void Process(const vector<cEntity*>& entities, float dt) = 0;
};