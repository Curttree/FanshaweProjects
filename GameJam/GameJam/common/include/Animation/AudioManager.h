#pragma once
#include <extern/FMOD/fmod.hpp>
#include <vector>
#include <extern/glm/vec3.hpp> // glm::vec3

class AudioManager {
public:
	AudioManager(void);
	~AudioManager(void);

	void StartUp(void);
	void ShutDown(void);

	//To be replaced with more scalable version.
	void PlayAudio(int index);

	void TimeStep(float deltaTime);
private:
	FMOD::System* _system = NULL;
	FMOD_RESULT _result = FMOD_OK;

	std::vector<FMOD::Sound*> _sounds;
	std::vector < FMOD::Channel*> _channels;
};