#include "configManager.h"
#include "cEntity.h"
#include "iEntity.h"
#include "cAIPlayer.h"
#include "globals.h"

configManager::configManager() {
    _actors = readJSONFile("actors.json");
    _objects = readJSONFile("objects.json");
    _sceneDescription = readJSONFile("scene.json");
    if (!_actors.HasParseError() && !_sceneDescription.HasParseError()) {
        initCamera();
        initProps();
    }
}

rapidjson::Document configManager::readJSONFile(std::string fileName) {
    std::ifstream inputFileStream(fileName);
    rapidjson::IStreamWrapper inputStreamWrapper(inputFileStream);
    rapidjson::Document result;

    result.ParseStream(inputStreamWrapper);
    if (result.HasParseError()) {
        std::cout <<"Config Error: Encountered an error when parsing " << fileName << std::endl;
        std::cout << "Please verify it is a valid json file. Otherwise the scene will not load." << std::endl;
    }
    return result;
}

void configManager::initCamera() {
    // Determine camera's starting position.
    if (_sceneDescription.HasMember("Camera")) {
        if (_sceneDescription["Camera"].HasMember("Position")) {
            float x, y, z;
            x = _sceneDescription["Camera"]["Position"]["x"].GetFloat();
            y = _sceneDescription["Camera"]["Position"]["y"].GetFloat();
            z = _sceneDescription["Camera"]["Position"]["z"].GetFloat();
            _cameraStartingPosition = glm::vec3(x, y, z);
        }
        //if (_sceneDescription["Camera"].HasMember("Orientation")) {
        //    float x, y, z;
        //    x = _sceneDescription["Camera"]["Orientation"]["x"].GetFloat();
        //    y = _sceneDescription["Camera"]["Orientation"]["y"].GetFloat();
        //    z = _sceneDescription["Camera"]["Orientation"]["z"].GetFloat();
        //    _cameraStartingOrientation = glm::vec3(x, y, z);
        //}
    }
    else {
        // Fall back to default start position if no camera info has been provided.
        _cameraStartingPosition = glm::vec3(0.f, 0.f, -4.f);
    }
}

void configManager::initPhysics() {
    // Determine boundaries.
    if (_sceneDescription.HasMember("Physics")) {
        // ASSUMPTION: If Physics is present all boundaries have been defined. 
        // If this is not always the case, this should be updated.
        float x, y, z;
        x = _sceneDescription["Physics"]["PositiveXBoundary"].GetFloat();
        y = _sceneDescription["Physics"]["PositiveYBoundary"].GetFloat();
        z = _sceneDescription["Physics"]["PositiveZBoundary"].GetFloat();
        _positiveBounds = glm::vec3(x, y, z);
        x = _sceneDescription["Physics"]["NegativeXBoundary"].GetFloat();
        y = _sceneDescription["Physics"]["NegativeYBoundary"].GetFloat();
        z = _sceneDescription["Physics"]["NegativeZBoundary"].GetFloat();
        _negativeBounds = glm::vec3(x, y, z);
    }
}


cMesh* configManager::initMesh(std::string meshName, int source) {
    cMesh* result = new cMesh();
    if (source == PROP) {
        if (_objects.HasMember(meshName.c_str())) {
            result->meshName = _objects[meshName.c_str()]["Model"].GetString();
            if (_objects[meshName.c_str()].HasMember("AlphaTransparency")) {
                result->alphaTransparency = _objects[meshName.c_str()]["AlphaTransparency"].GetFloat();
            }
            if (_objects[meshName.c_str()].HasMember("Scale")) {
                result->scale = _objects[meshName.c_str()]["Scale"].GetFloat();
            }
            if (_objects[meshName.c_str()].HasMember("NonUniformScale")) {
                result->nonUniformScale.x = _objects[meshName.c_str()]["NonUniformScale"]["x"].GetFloat();
                result->nonUniformScale.y = _objects[meshName.c_str()]["NonUniformScale"]["y"].GetFloat();
                result->nonUniformScale.z = _objects[meshName.c_str()]["NonUniformScale"]["z"].GetFloat();
            }
            if (_objects[meshName.c_str()].HasMember("SpecPower")) {
                result->wholeObjectShininess_SpecPower = _objects[meshName.c_str()]["SpecPower"].GetFloat();
            }
            if (_objects[meshName.c_str()].HasMember("Texture0")) {
                result->textureRatios[0] = 1.0f;
                result->textureNames[0] = _objects[meshName.c_str()]["Texture0"].GetString();
                if (_objects[meshName.c_str()].HasMember("Texture0Ratio")) {
                    result->textureRatios[0] = _objects[meshName.c_str()]["Texture0Ratio"].GetFloat();
                }
                else{
                    result->textureRatios[0] = 1.0f;
                }
            }
            if (_objects[meshName.c_str()].HasMember("Texture1")) {
                result->textureNames[1] = _objects[meshName.c_str()]["Texture1"].GetString();
                if (_objects[meshName.c_str()].HasMember("Texture1Ratio")) {
                    result->textureRatios[1] = _objects[meshName.c_str()]["Texture1Ratio"].GetFloat();
                }
                else {
                    result->textureRatios[1] = 1.0f;
                }
            }
            if (_objects[meshName.c_str()].HasMember("DiscardTexture")) {
                result->textureNames[8] = _objects[meshName.c_str()]["DiscardTexture"].GetString();
                result->bUseDiscardTransparency = true;
                result->textureRatios[8] = 1.0f;
            }
            if (_objects[meshName.c_str()].HasMember("MaskTexture0")) {
                result->textureRatios[4] = 1.0f;
                result->textureNames[4] = _objects[meshName.c_str()]["MaskTexture0"].GetString();
                result->textureNames[6] = _objects[meshName.c_str()]["MaskTextureFill0"].GetString();
            }
            if (_objects[meshName.c_str()].HasMember("DiscardColour")) {
                result->discardColour.r = _objects[meshName.c_str()]["DiscardColour"]["r"].GetFloat();
                result->discardColour.g = _objects[meshName.c_str()]["DiscardColour"]["g"].GetFloat();
                result->discardColour.b = _objects[meshName.c_str()]["DiscardColour"]["b"].GetFloat();
            }
            if (_objects[meshName.c_str()].HasMember("IsReflective")) {
                result->bDoesReflect = _objects[meshName.c_str()]["IsReflective"].GetBool();
            }
            if (_objects[meshName.c_str()].HasMember("RefractionIndex")) {
                result->refractionIndex = _objects[meshName.c_str()]["RefractionIndex"].GetFloat();
                result->bDoesRefract = true;
            }
            if (_objects[meshName.c_str()].HasMember("DiffuseColour")) {
                result->bUseWholeObjectDiffuseColour = true;
                result->wholeObjectDiffuseRGBA.r = _objects[meshName.c_str()]["DiffuseColour"]["r"].GetFloat();
                result->wholeObjectDiffuseRGBA.g = _objects[meshName.c_str()]["DiffuseColour"]["g"].GetFloat();
                result->wholeObjectDiffuseRGBA.b = _objects[meshName.c_str()]["DiffuseColour"]["b"].GetFloat();
                result->wholeObjectDiffuseRGBA.a = 1.f;
            }
        }
    }
    else if (source == ACTOR) {
        if (_actors.HasMember(meshName.c_str())) {
            result->meshName = _actors[meshName.c_str()]["Model"].GetString();
            if (_actors[meshName.c_str()].HasMember("AlphaTransparency")) {
                result->alphaTransparency = _actors[meshName.c_str()]["AlphaTransparency"].GetFloat();
            }
            if (_actors[meshName.c_str()].HasMember("Scale")) {
                result->scale = _actors[meshName.c_str()]["Scale"].GetFloat();
            }
            if (_actors[meshName.c_str()].HasMember("SpecPower")) {
                result->wholeObjectShininess_SpecPower = _actors[meshName.c_str()]["SpecPower"].GetFloat();
            }
            if (_actors[meshName.c_str()].HasMember("Texture0")) {
                result->textureRatios[0] = 1.0f;
                result->textureNames[0] = _actors[meshName.c_str()]["Texture0"].GetString();
            }
            if (_actors[meshName.c_str()].HasMember("MaskTexture0")) {
                result->textureRatios[4] = 1.0f;
                result->textureNames[4] = _actors[meshName.c_str()]["MaskTexture0"].GetString();
                rapidjson::GenericArray<false, rapidjson::Value> options = _actors[meshName.c_str()]["MaskTextureFill0_Options"].GetArray();
                result->textureNames[6] = options[mathHelper->getRandom(0, options.Size())].GetString();
            }
            if (_actors[meshName.c_str()].HasMember("MaskTexture1")) {
                result->textureRatios[5] = 1.0f;
                result->textureNames[5] = _actors[meshName.c_str()]["MaskTexture1"].GetString();
                rapidjson::GenericArray<false, rapidjson::Value> options = _actors[meshName.c_str()]["MaskTextureFill1_Options"].GetArray();
                result->textureNames[7] = options[mathHelper->getRandom(0, options.Size())].GetString();
            }
            if (_actors[meshName.c_str()].HasMember("SpecularMapTexture")) {
                result->bUseSpecularMap = true;
                result->specularMapTexture = _actors[meshName.c_str()]["SpecularMapTexture"].GetString();
            }
        }
    }

    // Check to see if we already have the model flagged to be loaded.
    if (result->meshName != "" && std::find(_modelsToLoad.begin(), _modelsToLoad.end(), result->meshName) == _modelsToLoad.end())
    {
        _modelsToLoad.push_back(result->meshName);
    }

    for (std::string name : result->textureNames) {
        if (name != "" && std::find(_texturesToLoad.begin(), _texturesToLoad.end(), name) == _texturesToLoad.end()) {
            _texturesToLoad.push_back(name);
        }
    }

    if (result->specularMapTexture != "" && std::find(_texturesToLoad.begin(), _texturesToLoad.end(), result->specularMapTexture) == _texturesToLoad.end()) {
        _texturesToLoad.push_back(result->specularMapTexture);
    }

    return result;
}

void configManager::initRink() {
    if (_sceneDescription.HasMember("Rink")) {
        if (_sceneDescription["Rink"].HasMember("Position")) {
            float x, y, z;
            x = _sceneDescription["Rink"]["Position"]["x"].GetFloat();
            y = _sceneDescription["Rink"]["Position"]["y"].GetFloat();
            z = _sceneDescription["Rink"]["Position"]["z"].GetFloat();
            _modelsFromConfigPosition = glm::vec3(x, y, z);
        }
        if (_sceneDescription["Rink"].HasMember("Components")) {
            rapidjson::GenericArray<false, rapidjson::Value> list = _sceneDescription["Rink"]["Components"].GetArray();
            cMesh* component = new cMesh();
            for (rapidjson::SizeType current = 0; current < list.Size(); current++) {
                component = initMesh(_sceneDescription["Rink"]["Components"][current].GetString());
                component->friendlyName = _sceneDescription["Rink"]["Components"][current].GetString();
                component->positionXYZ = _modelsFromConfigPosition; 
                _modelsFromConfig.push_back(component);
            }
        }
    }
    if (_sceneDescription.HasMember("GoalLights")) {
        cMesh* component = new cMesh();
        if (_sceneDescription["GoalLights"].HasMember("HomePosition")) {
            float x, y, z;
            x = _sceneDescription["GoalLights"]["HomePosition"]["x"].GetFloat();
            y = _sceneDescription["GoalLights"]["HomePosition"]["y"].GetFloat();
            z = _sceneDescription["GoalLights"]["HomePosition"]["z"].GetFloat();
            _homeGoalLightPosition = glm::vec3(x, y, z);
        }
        if (_sceneDescription["GoalLights"].HasMember("AwayPosition")) {
            float x, y, z;
            x = _sceneDescription["GoalLights"]["AwayPosition"]["x"].GetFloat();
            y = _sceneDescription["GoalLights"]["AwayPosition"]["y"].GetFloat();
            z = _sceneDescription["GoalLights"]["AwayPosition"]["z"].GetFloat();
            _awayGoalLightPosition = glm::vec3(x, y, z);
        }
        if (_sceneDescription["GoalLights"].HasMember("BaseModel")) {
            component = initMesh(_sceneDescription["GoalLights"]["BaseModel"].GetString());
            component->positionXYZ = _homeGoalLightPosition;
            _modelsFromConfig.push_back(component);
            component = initMesh(_sceneDescription["GoalLights"]["BaseModel"].GetString());
            component->positionXYZ = _awayGoalLightPosition;
            _modelsFromConfig.push_back(component);
        }
        if (_sceneDescription["GoalLights"].HasMember("BulbModel")) {
            component = initMesh(_sceneDescription["GoalLights"]["BulbModel"].GetString());
            component->positionXYZ = _homeGoalLightPosition;
            if (_sceneDescription["GoalLights"].HasMember("BulbOffset")) {
                component->positionXYZ.y += _sceneDescription["GoalLights"]["BulbOffset"].GetFloat();
            }
            _modelsFromConfig.push_back(component);
            component = initMesh(_sceneDescription["GoalLights"]["BulbModel"].GetString());
            component->positionXYZ = _awayGoalLightPosition;
            if (_sceneDescription["GoalLights"].HasMember("BulbOffset")) {
                component->positionXYZ.y += _sceneDescription["GoalLights"]["BulbOffset"].GetFloat();
            }
            _modelsFromConfig.push_back(component);
        }
    }
}

void configManager::initProps() {
    if (_sceneDescription.HasMember("Props")) {
        rapidjson::GenericArray<false, rapidjson::Value> list = _sceneDescription["Props"].GetArray();
        cMesh* component = new cMesh();
        float x, y, z;
        for (rapidjson::SizeType current = 0; current < list.Size(); current++) {
            // We assume the prop has a model name. If it doesn't it shouldn't be listed as a prop.
            component = initMesh(_sceneDescription["Props"][current]["Name"].GetString(), PROP);
            component->friendlyName = _sceneDescription["Props"][current]["Name"].GetString();
            if (_sceneDescription["Props"][current].HasMember("Position")) {
                    x = _sceneDescription["Props"][current]["Position"]["x"].GetFloat();
                    y = _sceneDescription["Props"][current]["Position"]["y"].GetFloat();
                    z = _sceneDescription["Props"][current]["Position"]["z"].GetFloat();
                    component->positionXYZ = glm::vec3(x, y, z);
            }
            if (_sceneDescription["Props"][current].HasMember("Orientation")) {
                x = glm::radians(_sceneDescription["Props"][current]["Orientation"]["x"].GetFloat());
                y = glm::radians(_sceneDescription["Props"][current]["Orientation"]["y"].GetFloat());
                z = glm::radians(_sceneDescription["Props"][current]["Orientation"]["z"].GetFloat());
                component->orientationXYZ = glm::vec3(x, y, z);
            }
            _modelsFromConfig.push_back(component);
        }
    }
    if (_sceneDescription.HasMember("Question4Monitors")) {
        rapidjson::GenericArray<false, rapidjson::Value> list = _sceneDescription["Question4Monitors"].GetArray();
        cMesh* component = new cMesh();
        float x, y, z;
        for (rapidjson::SizeType current = 0; current < list.Size(); current++) {
            // We assume the prop has a model name. If it doesn't it shouldn't be listed as a prop.
            component = initMesh(_sceneDescription["Question4Monitors"][current]["Name"].GetString(), PROP);
            component->friendlyName = _sceneDescription["Question4Monitors"][current]["Name"].GetString();
            if (_sceneDescription["Question4Monitors"][current].HasMember("Position")) {
                x = _sceneDescription["Question4Monitors"][current]["Position"]["x"].GetFloat();
                y = _sceneDescription["Question4Monitors"][current]["Position"]["y"].GetFloat();
                z = _sceneDescription["Question4Monitors"][current]["Position"]["z"].GetFloat();
                component->positionXYZ = glm::vec3(x, y, z);
            }
            if (_sceneDescription["Question4Monitors"][current].HasMember("Orientation")) {
                x = glm::radians(_sceneDescription["Question4Monitors"][current]["Orientation"]["x"].GetFloat());
                y = glm::radians(_sceneDescription["Question4Monitors"][current]["Orientation"]["y"].GetFloat());
                z = glm::radians(_sceneDescription["Question4Monitors"][current]["Orientation"]["z"].GetFloat());
                component->orientationXYZ = glm::vec3(x, y, z);
            }
            ::g_vec_pMonitorsQ4.push_back(component);
        }
    }
}

void configManager::initActors() {
    if (_sceneDescription.HasMember("Actors")) {
        rapidjson::GenericArray<false, rapidjson::Value> list = _sceneDescription["Actors"].GetArray();
        cMesh* component = new cMesh();
        float x, y, z;
        for (rapidjson::SizeType current = 0; current < list.Size(); current++) {
            // We assume the prop has a model name. If it doesn't it shouldn't be listed as a prop.
            component = initMesh(_sceneDescription["Actors"][current]["Name"].GetString(), ACTOR);
            component->friendlyName = _sceneDescription["Actors"][current]["Name"].GetString();
            if (_sceneDescription["Actors"][current].HasMember("Position")) {
                x = _sceneDescription["Actors"][current]["Position"]["x"].GetFloat();
                y = _sceneDescription["Actors"][current]["Position"]["y"].GetFloat();
                z = _sceneDescription["Actors"][current]["Position"]["z"].GetFloat();
                component->positionXYZ = glm::vec3(x, y, z);
            }
            //TODO: Right now we push the props to the rink vector. Decide if I want to track a separate actor vector.
            _modelsFromConfig.push_back(component);
            if (_sceneDescription["Actors"][current].HasMember("Type")) {
                // TODO: Make debug scale configurable (if it makes sense as the project evolves).
                iEntity* entity = new cAIPlayer(component, 10.f, 150.f);
                actorEntities.push_back(entity);
            }
            else {
                iEntity* entity = new cEntity(component, 5.f, 10.f);
                actorEntities.push_back(entity);
            }
        }
    }
}

void configManager::loadModelsIntoVAO(GLuint program, cVAOManager& gVAOManager, bool storeVertexData) {
    sModelDrawInfo currentModel;

    for (size_t x = 0; x < _modelsToLoad.size(); x++) {
        if (gVAOManager.LoadModelIntoVAO(_modelsToLoad[x], currentModel, program))
        {
            std::cout << "Loaded the model: " << currentModel.meshName << std::endl;
            std::cout << currentModel.numberOfVertices << " vertices loaded" << std::endl;
            std::cout << currentModel.numberOfTriangles << " triangles loaded" << std::endl;
            //  Add Model Draw Info to vector. We don't do much with it for now (the important thing is that it has been loaded into our VAOManager),
            //  but it could be useful in the future.
            _modelDrawInfo.push_back(currentModel);
        }
        else
        {
            std::cout << "Error: Didn't load the model OK" << std::endl;
        }
    }
}

void configManager::setupLights(cLightManager* lightManager, const int startIndex) {
    // Note this method can override lights that were previously declared. If the scope of this method grows,
    // I may wish to move the responsibility of this method into the light manager to streamline where/how lights are added.
    int theLightsIndex = startIndex;
    if (_sceneDescription.HasMember("Lights")) {
        rapidjson::GenericArray<false, rapidjson::Value> lightList = _sceneDescription["Lights"].GetArray();
        for (rapidjson::SizeType current = 0; current < lightList.Size(); current++) {
            theLightsIndex = current + startIndex;
            if (theLightsIndex >= lightManager->NUMBER_OF_LIGHTS) {
                std::cout << "Some lights have been skipped (not enough space declared)." << std::endl;
                return;
            }
            if (_sceneDescription["Lights"][current].HasMember("Type")) {
                lightManager->theLights[theLightsIndex].param1.x = _sceneDescription["Lights"][current]["Type"].GetFloat();
            }
            if (_sceneDescription["Lights"][current].HasMember("Position")) {
                lightManager->theLights[theLightsIndex].position.x = _sceneDescription["Lights"][current]["Position"]["x"].GetFloat();
                lightManager->theLights[theLightsIndex].position.y = _sceneDescription["Lights"][current]["Position"]["y"].GetFloat();
                lightManager->theLights[theLightsIndex].position.z = _sceneDescription["Lights"][current]["Position"]["z"].GetFloat();
            }
            if (_sceneDescription["Lights"][current].HasMember("Direction")) {
                lightManager->theLights[theLightsIndex].direction.x = _sceneDescription["Lights"][current]["Direction"]["x"].GetFloat();
                lightManager->theLights[theLightsIndex].direction.y = _sceneDescription["Lights"][current]["Direction"]["y"].GetFloat();
                lightManager->theLights[theLightsIndex].direction.z = _sceneDescription["Lights"][current]["Direction"]["z"].GetFloat();
            }
            if (_sceneDescription["Lights"][current].HasMember("Attenuation")) {
                lightManager->theLights[theLightsIndex].atten.x = _sceneDescription["Lights"][current]["Attenuation"]["Constant"].GetFloat();
                lightManager->theLights[theLightsIndex].atten.y = _sceneDescription["Lights"][current]["Attenuation"]["Linear"].GetFloat();
                lightManager->theLights[theLightsIndex].atten.z = _sceneDescription["Lights"][current]["Attenuation"]["Quadratic"].GetFloat();
                lightManager->theLights[theLightsIndex].atten.w = _sceneDescription["Lights"][current]["Attenuation"]["DistanceCutOff"].GetFloat();
            }
            if (_sceneDescription["Lights"][current].HasMember("Diffuse")) {
                lightManager->theLights[theLightsIndex].diffuse.r = _sceneDescription["Lights"][current]["Diffuse"]["r"].GetFloat();
                lightManager->theLights[theLightsIndex].diffuse.g = _sceneDescription["Lights"][current]["Diffuse"]["g"].GetFloat();
                lightManager->theLights[theLightsIndex].diffuse.b = _sceneDescription["Lights"][current]["Diffuse"]["b"].GetFloat();
                lightManager->theLights[theLightsIndex].diffuse.a = _sceneDescription["Lights"][current]["Diffuse"]["a"].GetFloat();
            }
            if (_sceneDescription["Lights"][current].HasMember("Specular")) {
                lightManager->theLights[theLightsIndex].specular.r = _sceneDescription["Lights"][current]["Specular"]["r"].GetFloat();
                lightManager->theLights[theLightsIndex].specular.g = _sceneDescription["Lights"][current]["Specular"]["g"].GetFloat();
                lightManager->theLights[theLightsIndex].specular.b = _sceneDescription["Lights"][current]["Specular"]["b"].GetFloat();
                lightManager->theLights[theLightsIndex].specular.a = _sceneDescription["Lights"][current]["Specular"]["a"].GetFloat();
            }
            if (_sceneDescription["Lights"][current].HasMember("InnerAngle")) {
                lightManager->theLights[theLightsIndex].param1.y = _sceneDescription["Lights"][current]["InnerAngle"].GetFloat();
            }
            if (_sceneDescription["Lights"][current].HasMember("OuterAngle")) {
                lightManager->theLights[theLightsIndex].param1.z = _sceneDescription["Lights"][current]["OuterAngle"].GetFloat();
            }
            lightManager->TurnOnLight(theLightsIndex);
        }
    }
}