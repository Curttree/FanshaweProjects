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
#include "cEntity.h"
#include "Graphics/cVAOManager.h"
#include <Graphics/cLightManager.h>

#define PROP 0
#define ACTOR 1

class configManager {
public:
	rapidjson::Document _actors;
	rapidjson::Document _objects;
	rapidjson::Document _sceneDescription;
	rapidjson::Document _physicsDescription;
	glm::vec3 _shotPositions[3] = {glm::vec3(0.f),glm::vec3(0.f), glm::vec3(0.f) };
	glm::vec3 _cameraOffset = glm::vec3(0.f);
	glm::vec3 _rinkPosition = glm::vec3(0.f);
	glm::vec3 _homeGoalLightPosition = glm::vec3(0.f);
	glm::vec3 _awayGoalLightPosition = glm::vec3(0.f);
	std::vector<cMesh*> _rink;
	std::vector<std::string> _texturesToLoad;
	std::vector<sModelDrawInfo> _modelDrawInfo;
	glm::vec3 _positiveBounds;
	glm::vec3 _negativeBounds; 
	float _cornerRadius = 0.f;
	int _samplePoints = 0;
	std::vector<iEntity*> actorEntities;
	configManager(); 
	void loadModelsIntoVAO(GLuint program, cVAOManager& gVAOManager, bool storeVertexData = false);
	void setupLights(cLightManager* lightManager, const int startIndex);
	std::map<std::string, std::vector<sVertex>> _vertexData;

private:
	float _radius;
	float _bodyHeight;
	std::vector<std::string> _modelsToLoad;
	rapidjson::Document readJSONFile(std::string fileName);
	void initActors();
	void initRink();
	void initProps();
	void initPhysics();
	cMesh* initMesh(std::string meshName,int source = PROP);
	cMathHelper* mathHelper = cMathHelper::Instance();
};