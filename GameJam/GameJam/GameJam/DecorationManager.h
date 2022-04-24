#pragma once
#include <string>
#include "cParticleEmitter.h"

//Decorate scene with models with no physics properties.
class DecorationManager {
public:
	static DecorationManager* Instance();
	void DecorateScene(); 
	void TimeStep(float deltaTime);
private:
	std::vector<int> leftVents;
	std::vector<int> rightVents;
	std::vector<int> leftLights;
	std::vector<int> rightLights;
	DecorationManager();
	void AddVents();
	void BuildStreet(int startZ, int endZ);
	void BuildStreetsAhead(float starting);
	bool ShouldAddVent(int location, bool onLeft);
	static DecorationManager* _instance;
	void GetBuildingParts(std::string& door, std::string& stack, std::string& roof);
	std::vector<cParticleEmitter*> vents;
};