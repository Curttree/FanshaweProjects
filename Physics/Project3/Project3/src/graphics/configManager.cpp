#include "configManager.h"

configManager::configManager(sCannonDef* _cannonDef) {
    _modelsDoc = readJSONFile("models.json");
    _sceneDescription = readJSONFile("scene.json");
    if (!_modelsDoc.HasParseError() && !_sceneDescription.HasParseError()) {
        initCamera();
        initCannon(_cannonDef);
        initProjectiles();
        initModels();
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

void configManager::initCannon(sCannonDef* cannon) {
    float degrees;

    if (_sceneDescription.HasMember("Cannon")) {
        if (_sceneDescription["Cannon"].HasMember("Pitch")) {
            if (_sceneDescription["Cannon"]["Pitch"].HasMember("MinDegrees")) {
                degrees = _sceneDescription["Cannon"]["Pitch"]["MinDegrees"].GetFloat();
                cannon->lowerPitch = glm::radians(degrees);
            }
            if (_sceneDescription["Cannon"]["Pitch"].HasMember("MaxDegrees")) {
                degrees = _sceneDescription["Cannon"]["Pitch"]["MaxDegrees"].GetFloat();
                cannon->upperPitch = glm::radians(degrees);
            }
        }
        if (_sceneDescription["Cannon"].HasMember("Yaw")) {
            if (_sceneDescription["Cannon"]["Yaw"].HasMember("MinDegrees")) {
                degrees = _sceneDescription["Cannon"]["Yaw"]["MinDegrees"].GetFloat();
                cannon->lowerYaw = glm::radians(degrees);
            }
            if (_sceneDescription["Cannon"]["Yaw"].HasMember("MaxDegrees")) {
                degrees = _sceneDescription["Cannon"]["Yaw"]["MaxDegrees"].GetFloat();
                cannon->upperYaw = glm::radians(degrees);
            }
        }
    }
}

void configManager::initProjectiles() {
    std::string projectileName;
    if (_sceneDescription.HasMember("Projectiles")) {
        if (_sceneDescription["Projectiles"].HasMember("Bullet")) {
            projectileName = "Bullet";
            projectileParser(projectileName.c_str(), BULLET_PROJ);
        }
        if (_sceneDescription["Projectiles"].HasMember("Laser")) {
            projectileName = "Laser";
            projectileParser(projectileName.c_str(), LASER_PROJ);
        }
        if (_sceneDescription["Projectiles"].HasMember("CannonBall")) {
            projectileName = "CannonBall";
            projectileParser(projectileName.c_str(), CANNON_PROJ);
        }
        if (_sceneDescription["Projectiles"].HasMember("EnergyBall")) {
            projectileName = "EnergyBall";
            projectileParser(projectileName.c_str(), ENERGY_PROJ);
        }
    }
}

void configManager::projectileParser(const char* name, int type) {
    sParticleTemplate* candidate = new sParticleTemplate();
    if (_sceneDescription["Projectiles"][name].HasMember("Radius")) {
        candidate->radius = _sceneDescription["Projectiles"][name]["Radius"].GetFloat();
    }
    if (_sceneDescription["Projectiles"][name].HasMember("Damping")) {
        candidate->damping = _sceneDescription["Projectiles"][name]["Damping"].GetFloat();
    }
    if (_sceneDescription["Projectiles"][name].HasMember("Density")) {
        candidate->density = _sceneDescription["Projectiles"][name]["Density"].GetFloat();
    }
    if (_sceneDescription["Projectiles"][name].HasMember("MuzzleVelocity")) {
        candidate->muzzleVelocity = _sceneDescription["Projectiles"][name]["MuzzleVelocity"].GetFloat();
    }
    if (_sceneDescription["Projectiles"][name].HasMember("TimeLimit")) {
        candidate->timeLimit = _sceneDescription["Projectiles"][name]["TimeLimit"].GetFloat();
    }
    if (_sceneDescription["Projectiles"][name].HasMember("DistanceLimit")) {
        candidate->distanceLimit = _sceneDescription["Projectiles"][name]["DistanceLimit"].GetFloat();
    }
    if (_sceneDescription["Projectiles"][name].HasMember("ApplyGravity")) {
        candidate->applyGravity = _sceneDescription["Projectiles"][name]["ApplyGravity"].GetBool();
    }
    if (_sceneDescription["Projectiles"][name].HasMember("Accelerate")) {
        candidate->accelerate = _sceneDescription["Projectiles"][name]["Accelerate"].GetBool();
    }
    if (_sceneDescription["Projectiles"][name].HasMember("Colour")) {
        // Even if they declare colour, don't override unless a r,g, or b value has been declared.
        if (_sceneDescription["Projectiles"][name]["Colour"].HasMember("r") || _sceneDescription["Projectiles"][name]["Colour"].HasMember("g") || _sceneDescription["Projectiles"][name]["Colour"].HasMember("b")) {
            float r = 0.f, g = 0.f, b = 0.f;
            if (_sceneDescription["Projectiles"][name]["Colour"].HasMember("r")) {
                r = _sceneDescription["Projectiles"][name]["Colour"]["r"].GetFloat();
            }
            if (_sceneDescription["Projectiles"][name]["Colour"].HasMember("g")) {
                g = _sceneDescription["Projectiles"][name]["Colour"]["g"].GetFloat();
            }
            if (_sceneDescription["Projectiles"][name]["Colour"].HasMember("b")) {
                b = _sceneDescription["Projectiles"][name]["Colour"]["b"].GetFloat();
            }
            candidate->colour = glm::vec3(r,g,b);
        }
    }
    projectileFactory->InitProjectile(type, candidate);
}

void configManager::initCamera() {
    // Determine camera's starting position.
    if (_sceneDescription.HasMember("Camera") && _sceneDescription["Camera"].HasMember("Position")) {
        float x, y, z;
        x = _sceneDescription["Camera"]["Position"]["x"].GetFloat();
        y = _sceneDescription["Camera"]["Position"]["y"].GetFloat();
        z = _sceneDescription["Camera"]["Position"]["z"].GetFloat();
        _cameraStartingPosition = glm::vec3(x, y, z);
    }
    else {
        // Fall back to default start position if no camera info has been provided.
        _cameraStartingPosition = glm::vec3(0.f, 0.f, -4.f);
    }
    if (_sceneDescription.HasMember("Camera") && _sceneDescription["Camera"].HasMember("Focus")) {
        float x, y, z;
        x = _sceneDescription["Camera"]["Focus"]["x"].GetFloat();
        y = _sceneDescription["Camera"]["Focus"]["y"].GetFloat();
        z = _sceneDescription["Camera"]["Focus"]["z"].GetFloat();
        _cameraStartingFocus = glm::vec3(x, y, z);
    }
    else {
        // Fall back to default focus if no camera info has been provided.
        _cameraStartingFocus = glm::vec3(0.f, 0.f, 0.f);
    }

}

void configManager::initModels() {
    //Handle any miscellaneous models that we wish to load. 
    if (_modelsDoc.HasMember("Misc") && _modelsDoc["Misc"].IsArray()) {
        rapidjson::GenericArray<false, rapidjson::Value> models = _modelsDoc["Misc"].GetArray();
        for (rapidjson::SizeType current = 0; current < models.Size(); current++) {
            _modelsToLoad.push_back(_modelsDoc["Misc"][current].GetString());
        }
    }
}

void configManager::loadModelsIntoVAO(GLuint program, cVAOManager& gVAOManager) {
    sModelDrawInfo currentModel;

    for (size_t x = 0; x < _modelsToLoad.size(); x++) {
        if (gVAOManager.LoadModelIntoVAO(_modelsToLoad[x], currentModel, program))
        {
            std::cout << "Loaded the model: " << currentModel.modelName << std::endl;
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