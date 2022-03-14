#pragma onc
#include "cBunker.h"

class cSplitBunker : public cBunker {
public:
	cSplitBunker(float posX, float posY, float angle, float scale = 1.f);
	virtual void Fire();
};