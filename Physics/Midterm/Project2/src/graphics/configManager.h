#pragma once
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>

#include "extern_includes.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "cModel.h"
#include "cVAOManager.h"


class configManager {
public:
	rapidjson::Document _modelsDoc;
	rapidjson::Document _sceneDescription;
	glm::vec3 _cameraStartingPosition;
	glm::vec3 _cameraStartingFocus;
	std::vector<sModelDrawInfo> _modelDrawInfo;
	configManager(); 
	void loadModelsIntoVAO(GLuint program, cVAOManager& gVAOManager);

private:
	float _radius;
	float _bodyHeight;
	std::vector<std::string> _modelsToLoad;
	rapidjson::Document readJSONFile(std::string fileName);
	void initCamera();
	void initModels();
};