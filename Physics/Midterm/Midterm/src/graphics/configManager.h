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
#include "../../cProjectileFactory.h"
#include "../../sCannonDef.h"


class configManager {
public:
	rapidjson::Document _modelsDoc;
	rapidjson::Document _sceneDescription;
	glm::vec3 _cameraStartingPosition;
	glm::vec3 _cameraStartingFocus;
	std::vector<sModelDrawInfo> _modelDrawInfo;
	configManager(sCannonDef* _cannonDef); 
	void loadModelsIntoVAO(GLuint program, cVAOManager& gVAOManager);
private:
	cProjectileFactory* projectileFactory = cProjectileFactory::Instance();
	std::vector<std::string> _modelsToLoad;
	rapidjson::Document readJSONFile(std::string fileName);
	void initCamera();
	void initModels();
	void initProjectiles();
	void initCannon(sCannonDef* cannon); 
	void projectileParser(const char* name, int type);
};