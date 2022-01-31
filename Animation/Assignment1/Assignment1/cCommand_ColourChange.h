#pragma once
#include "iCommand.h"
#include <string>
#include "cMesh.h"

class cCommand_ColourChange : public iCommand {
public:
	cCommand_ColourChange(cMesh* _mesh, const bool _revertToDefault, glm::vec3 _colour = glm::vec3(1.f));
	virtual void Execute();
private:
	cMesh* mesh; 
	const bool revertToDefault;
	glm::vec3 colour = glm::vec3(1.f);
};