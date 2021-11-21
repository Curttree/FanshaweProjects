#include "configManager.h"

configManager::configManager() {
    _actors = readJSONFile("actors.json");
    _sceneDescription = readJSONFile("scene.json");
    if (!_actors.HasParseError() && !_sceneDescription.HasParseError()) {
        initCamera();
        initBody();
        initAudience();
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
        _body = initMesh(_sceneDescription["DeadBody"]["Actor"].GetString(),false);
    }
    if (_sceneDescription.HasMember("DeadBody") && _sceneDescription["DeadBody"].HasMember("Radius")) {
        _radius = _sceneDescription["DeadBody"]["Radius"].GetFloat();
    }
    if (_sceneDescription.HasMember("DeadBody") && _sceneDescription["DeadBody"].HasMember("Height")) {
        _bodyHeight = _sceneDescription["DeadBody"]["Height"].GetFloat();
    }
}

cMesh* configManager::initMesh(std::string actorName, bool isAlive) {
    cMesh* result = new cMesh();
    if (_actors.HasMember(actorName.c_str())) {
        if (isAlive) {
            result->meshName = _actors[actorName.c_str()]["AliveModel"].GetString();
        }
        else {
            result->meshName = _actors[actorName.c_str()]["DeadModel"].GetString();
            result->positionXYZ = _bodyPosition;
            // Rotate body so it is laying down on it's back.
            result->orientationXYZ.z = 1.571f;
            result->orientationXYZ.y = 1.571f;
        }
        if (_actors[actorName.c_str()].HasMember("Scale")) {
            result->scale = _actors[actorName.c_str()]["Scale"].GetFloat();
        }
    }
    // By default, all mesh objects will have their colors set by the program, and will be filled.
    result->bUseWholeObjectDiffuseColour = true;
    result->bIsWireframe = false;

    // Check to see if we already have the model flagged to be loaded.
    if (std::find(_modelsToLoad.begin(), _modelsToLoad.end(), result->meshName) == _modelsToLoad.end())
    {
        _modelsToLoad.push_back(result->meshName);
    }

    return result;
}

void configManager::initAudience() {
    if (_sceneDescription.HasMember("Audience")) {
        rapidjson::GenericArray<false,rapidjson::Value> list = _sceneDescription["Audience"].GetArray();
        std::string answer;
        const float pi = 3.14159265358979323846f;       // Approx value of pi for calculation (can replace with value from math library if require additional precision).
        float test;
        cMesh* actor = new cMesh();
        for (rapidjson::SizeType current = 0; current < list.Size(); current++)
        {
            actor = initMesh(_sceneDescription["Audience"][current].GetString());
            // Determine relative position to the body using mathematical formula for equidistant points on a circle.
            actor->positionXYZ.x = _bodyPosition.x - _bodyHeight/2 + _radius * cos(2 * pi * current /list.Size());     // Since center of gravity for the model is seemingly located at the foot of the model, offset x to try to centralize the body.
            actor->positionXYZ.z = _bodyPosition.z + _radius * sin(2 * pi * current / list.Size());
            actor->orientationXYZ.y = (1.5 * pi) - (float)(current) / list.Size() * (2.f * pi);    // Rotate around y axis so they are roughly facing the center of the circle. 1st actor required a rotation of 1.5 pi, others should be rotated accordingly based on their placement.
            _audience.push_back(actor);
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