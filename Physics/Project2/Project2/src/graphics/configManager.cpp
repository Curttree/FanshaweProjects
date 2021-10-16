#include "configManager.h"

configManager::configManager() {
    std::stringstream modelsStream;
    std::stringstream sceneStream;
    modelsStream << PROJECT_DIR << "models.json";
    sceneStream << PROJECT_DIR << "scene.json";
    _modelsDoc = readJSONFile(modelsStream.str());
    _sceneDescription = readJSONFile(sceneStream.str());
    if (!_modelsDoc.HasParseError() && !_sceneDescription.HasParseError()) {
        initCamera();
        initAudience();
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

cModel configManager::initMesh(std::string actorName, bool isAlive) {
    cModel result;
    // TODO: Remove temp hack
    result.modelName = "assets/pokeball.ply";
    // By default, all mesh objects will have their colors set by the program, and will be filled.
    result.bOverriveVertexColourHACK = true;
    result.bIsWireframe = false;

    // Check to see if we already have the model flagged to be loaded.
    if (std::find(_modelsToLoad.begin(), _modelsToLoad.end(), result.modelName) == _modelsToLoad.end())
    {
        _modelsToLoad.push_back(result.modelName);
    }

    return result;
}

//TODO: Remove and replace with dynamic generation of fireworks.
void configManager::initAudience() {
    if (_sceneDescription.HasMember("Audience")) {
        rapidjson::GenericArray<false,rapidjson::Value> list = _sceneDescription["Audience"].GetArray();
        std::string answer;
        const float pi = 3.14159265358979323846f;       // Approx value of pi for calculation (can replace with value from math library if require additional precision).
        float test;
        cModel actor;
        for (rapidjson::SizeType current = 0; current < list.Size(); current++)
        {
            actor = initMesh(_sceneDescription["Audience"][current].GetString());
            // Determine relative position to the body using mathematical formula for equidistant points on a circle.
            actor.positionXYZ.x = _bodyPosition.x - _bodyHeight/2 + _radius * cos(2 * pi * current /list.Size());     // Since center of gravity for the model is seemingly located at the foot of the model, offset x to try to centralize the body.
            actor.positionXYZ.z = _bodyPosition.z + _radius * sin(2 * pi * current / list.Size());
            actor.orientationXYZ.y = (1.5 * pi) - (float)(current) / list.Size() * (2.f * pi);    // Rotate around y axis so they are roughly facing the center of the circle. 1st actor required a rotation of 1.5 pi, others should be rotated accordingly based on their placement.
            _audience.push_back(actor);
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