#include "configManager.h"

configManager::configManager() {
    _actors = readJSONFile("actors.json");
    _objects = readJSONFile("objects.json");
    _sceneDescription = readJSONFile("scene.json");
    if (!_actors.HasParseError() && !_sceneDescription.HasParseError()) {
        initCamera();
        initRink();
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
        float x, y, z;
        x = _sceneDescription["Camera"]["x"].GetFloat();
        y = _sceneDescription["Camera"]["y"].GetFloat();
        z = _sceneDescription["Camera"]["z"].GetFloat();
        _cameraStartingPosition = glm::vec3(x, y, z);
    }
    else {
        // Fall back to default start position if no camera info has been provided.
        _cameraStartingPosition = glm::vec3(0.f, 0.f, -4.f);
    }
}

void configManager::initBody() {
    if (_sceneDescription.HasMember("DeadBody") && _sceneDescription["DeadBody"].HasMember("Position")) {
        float x, y, z;
        x = _sceneDescription["DeadBody"]["Position"]["x"].GetFloat();
        y = _sceneDescription["DeadBody"]["Position"]["y"].GetFloat();
        z = _sceneDescription["DeadBody"]["Position"]["z"].GetFloat();
        _bodyPosition = glm::vec3(x, y, z);
    }
    else {
        // Fall back to default start position if no camera info has been provided.
        _bodyPosition = glm::vec3(0.f, 0.f, 0.f);
    }
    if (_sceneDescription.HasMember("DeadBody") && _sceneDescription["DeadBody"].HasMember("Actor")) {
        _body = initMesh(_sceneDescription["DeadBody"]["Actor"].GetString());
    }
    if (_sceneDescription.HasMember("DeadBody") && _sceneDescription["DeadBody"].HasMember("Radius")) {
        _radius = _sceneDescription["DeadBody"]["Radius"].GetFloat();
    }
    if (_sceneDescription.HasMember("DeadBody") && _sceneDescription["DeadBody"].HasMember("Height")) {
        _bodyHeight = _sceneDescription["DeadBody"]["Height"].GetFloat();
    }
}

cMesh* configManager::initMesh(std::string meshName) {
    cMesh* result = new cMesh();
    if (_objects.HasMember(meshName.c_str())) {
        result->meshName = _objects[meshName.c_str()]["Model"].GetString();
    }

    // Check to see if we already have the model flagged to be loaded.
    if (std::find(_modelsToLoad.begin(), _modelsToLoad.end(), result->meshName) == _modelsToLoad.end())
    {
        _modelsToLoad.push_back(result->meshName);
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
            _rinkPosition = glm::vec3(x, y, z);
        }
        if (_sceneDescription["Rink"].HasMember("Components")) {
            rapidjson::GenericArray<false, rapidjson::Value> list = _sceneDescription["Rink"]["Components"].GetArray();
            cMesh* component = new cMesh();
            for (rapidjson::SizeType current = 0; current < list.Size(); current++) {
                component = initMesh(_sceneDescription["Rink"]["Components"][current].GetString());
                component->positionXYZ = _rinkPosition; 
                _rink.push_back(component);
            }
        }
    }
}

void configManager::loadAudienceIntoVAO(GLuint program, cVAOManager& gVAOManager) {
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