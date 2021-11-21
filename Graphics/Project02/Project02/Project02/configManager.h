#pragma once
#include <extern/rapidjson/document.h>
#include <extern/rapidjson/writer.h>
#include <extern/rapidjson/stringbuffer.h>
#include <extern/rapidjson/istreamwrapper.h>

#include "Graphics/GLCommon.h"

#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "cMesh.h"
#include "Graphics/cVAOManager.h"


class configManager {
public:
	rapidjson::Document _actors;
	rapidjson::Document _objects;
	rapidjson::Document _sceneDescription;
	glm::vec3 _cameraStartingPosition;
	glm::vec3 _bodyPosition;
	glm::vec3 _rinkPosition = glm::vec3(0.f);
	std::vector<cMesh*> _audience;
	std::vector<cMesh*> _rink;
	cMesh* _body;
	std::vector<sModelDrawInfo> _modelDrawInfo;
	configManager(); 
	void loadAudienceIntoVAO(GLuint program, cVAOManager& gVAOManager);

private:
	float _radius;
	float _bodyHeight;
	std::vector<std::string> _modelsToLoad;
	rapidjson::Document readJSONFile(std::string fileName);
	void initCamera();
	void initBody();
	void initRink();
	cMesh* initMesh(std::string meshName);
};