#pragma once
#include "cFirework.h"
#include "src/graphics/cModel.h"

struct sFireworkObject {
public:
	virtual ~sFireworkObject();
	cFirework* particle;
	cModel* model;
};