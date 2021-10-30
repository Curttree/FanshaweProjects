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