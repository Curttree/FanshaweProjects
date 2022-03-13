#include "stdafx.h"
#include "cStructureFactory.h"

// Structure Types
#include "cFuelPickup.h"
#include "cBunker.h"

cStructureFactory* cStructureFactory::_instance = 0;
cStructureFactory::cStructureFactory() {

}
cStructureFactory* cStructureFactory::Instance() {
	if (_instance == 0) {
		_instance = new cStructureFactory;
	}

	return _instance;
}

cStructure* cStructureFactory::BuildStructure(int type, Vec2 position, float angle) {
	cStructure* structure = 0;
	switch (type) {
	case(0):
		structure = new cFuelPickup(position.x, position.y, angle);
		break;
	case(1):
		structure = new cBunker(position.x, position.y, angle);
		break;
	default:
		// Unknown projectile. Return empty pointer.
		break;
	}
	return structure;
}