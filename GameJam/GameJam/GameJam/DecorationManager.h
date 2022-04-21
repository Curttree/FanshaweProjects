#pragma once
#include <string>

//Decorate scene with models with no physics properties.
class DecorationManager {
public:
	static DecorationManager* Instance();
	void DecorateScene(); 
private:
	DecorationManager();
	static DecorationManager* _instance;
	void GetBuildingParts(std::string& door, std::string& stack, std::string& roof);
};