#include "AudioManager.h"
#include "globals.h"
#include <string>

AudioManager::AudioManager(void)
{
    _sounds.resize(4);
    _channels.resize(4);
}

AudioManager::~AudioManager(void)
{
}

void AudioManager::StartUp(void)
{
    _result = FMOD::System_Create(&_system);
    if (_result != FMOD_OK) {
        fprintf(stderr, "Unable to create FMOD system");
        return;
    }

    _result = _system->init(32, FMOD_INIT_NORMAL, NULL);
    if (_result != FMOD_OK) {
        fprintf(stderr, "Unable to initialize FMOD system");
        return;
    }

    //Create a sample.	
    _result = _system->createSound("assets\\sounds\\crash-bandicoot-sounds-woah.mp3", FMOD_LOOP_OFF, 0, &_sounds[0]);
    _result = _system->createSound("assets\\sounds\\among-us-death-animation-4.mp3", FMOD_LOOP_OFF, 0, &_sounds[1]);
    _result = _system->createSound("assets\\sounds\\tindeck_1.mp3", FMOD_LOOP_OFF, 0, &_sounds[2]); 
    _result = _system->createSound("assets\\sounds\\morganpurkis-rustling-grass-2.wav", FMOD_LOOP_OFF, 0, &_sounds[3]);
    if (_result != FMOD_OK) {
        fprintf(stderr, "Unable to create sound");
        return;
    }

    return;

}

void AudioManager::ShutDown(void)
{
    for (FMOD::Sound* sound : _sounds) {
        _result = sound->release();
        if (_result != FMOD_OK) {
            std::string output = "Unable to release sound 1";
            fprintf(stderr, output.c_str());
        }
    }
    _result = _system->close();
    if (_result != FMOD_OK) {
        fprintf(stderr, "Unable to close system");
    }

    _result = _system->release();
    if (_result != FMOD_OK) {
        fprintf(stderr, "Unable to release system");
    }
}

void AudioManager::TimeStep(float deltaTime) {
}

void AudioManager::PlayAudio(int index) {
    bool isPlaying;
    _channels[index]->isPlaying(&isPlaying);
    if (isPlaying || index >= _sounds.size()) {
        //Sound is already playing, or doesn't exist. Don't overlap.
        return;
    }

    _result = _system->playSound(_sounds[index], 0, false, &_channels[index]);
    if (_result != FMOD_OK) {
        std::string output = "Unable to play sound";
        fprintf(stderr, output.c_str());
        return;
    }
}