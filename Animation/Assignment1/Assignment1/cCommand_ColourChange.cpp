#include "cCommand_ColourChange.h"
#include "globals.h"
#include <iostream>

cCommand_ColourChange::cCommand_ColourChange(cMesh* _mesh, const bool _revertToDefault, glm::vec3 _colour) : revertToDefault(_revertToDefault) {
	mesh = _mesh; 
	colour = _colour;
}

void cCommand_ColourChange::Execute() {
	if (revertToDefault || !::g_pGameEngine->showColourChanges) {
		// We no longer wish to override the colour of the object.
		// OR...the user has toggled off the colour changes.
		mesh->bDontLight = false;
		mesh->bUseObjectDebugColour = false;
		return;
	}

	mesh->bDontLight = true;
	mesh->bUseObjectDebugColour = true;
	mesh->objectDebugColourRGBA = glm::vec4(colour, 1.f);
}