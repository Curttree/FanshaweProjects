#include "AudioManager.h"
#include "../../../GameJam/globals.h"
#include <string>
#include "../../../GameJam/soundDefs.h"
#include <iostream>

AudioManager::AudioManager(void)
{
    _sounds.resize(4);
    _channels.resize(4);
}

AudioManager::~AudioManager(void)
{
    //for (FMOD::Sound* sound : _sounds) {
    for (unsigned int index = 0; index < _sounds.size(); index++) {
        _result = _sounds[index]->release();
        if (_result != FMOD_OK) {
            std::string output = "Unable to release sound";
            fprintf(stderr, output.c_str());
            std::cout << std::endl;
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
    _result = _system->createSound("assets\\sounds\\cans.mp3", FMOD_LOOP_OFF, 0, &_sounds[0]);
    _result = _system->createSound("assets\\sounds\\gun.wav", FMOD_LOOP_OFF, 0, &_sounds[1]);
    _result = _system->createStream("assets\\sounds\\ambience.wav", FMOD_LOOP_NORMAL, 0, &_sounds[2]);
    _result = _system->createSound("assets\\sounds\\ping.wav", FMOD_LOOP_OFF, 0, &_sounds[3]);
    if (_result != FMOD_OK) {
        fprintf(stderr, "Unable to create sound");
        return;
    }

    return;

}

void AudioManager::ShutDown(void)
{
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