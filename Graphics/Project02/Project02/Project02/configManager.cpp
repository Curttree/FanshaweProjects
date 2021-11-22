#include "configManager.h"

configManager::configManager() {
    _actors = readJSONFile("actors.json");
    _objects = readJSONFile("objects.json");
    _sceneDescription = readJSONFile("scene.json");
    if (!_actors.HasParseError() && !_sceneDescription.HasParseError()) {
        initCamera();
        initRink();
        initProps();
        initActors();
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
        }
    }

    // Check to see if we already have the model flagged to be loaded.
    if (result->meshName != "" && std::find(_modelsToLoad.begin(), _modelsToLoad.end(), result->meshName) == _modelsToLoad.end())
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
                component->friendlyName = _sceneDescription["Rink"]["Components"][current].GetString();
                component->positionXYZ = _rinkPosition; 
                _rink.push_back(component);
            }
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
            //TODO: Right now we push the props to the rink vector. Decide if I want to track a separate prop vector.
            _rink.push_back(component);
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
            _rink.push_back(component);
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