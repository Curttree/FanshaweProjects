#include "AudioManager.h"
#include "globals.h"
#include <string>

AudioManager::AudioManager(void)
{
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
    _result = _system->createSound("assets\\sounds\\billiardball.wav", FMOD_3D, 0, &_sound);

    if (_result != FMOD_OK) {
        fprintf(stderr, "Unable to create sound");
        return;
    }
    _result = _sound->set3DMinMaxDistance(0.5f, 25.0f);
    if (_result != FMOD_OK) {
        fprintf(stderr, "Unable to set 3D properties");
        return;
    }
    _result = _channel->setVolume(0.9f);
    if (_result != FMOD_OK) {
        fprintf(stderr, "Unable to set volume");
        return;
    }
    _result = _sound->setMode(FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
    if (_result != FMOD_OK) {
        std::string output = "Unable to set loop mode to normal for sound 1";
        fprintf(stderr, output.c_str());
        return;
    }

    return;

}

void AudioManager::ShutDown(void)
{
    _result = _sound->release();
    if (_result != FMOD_OK) {
        std::string output = "Unable to release sound 1";
        fprintf(stderr, output.c_str());
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

void AudioManager::PlayAudio(glm::vec3 position) {
    bool isPlaying;
    _channel->isPlaying(&isPlaying);
    if (isPlaying) {
        //Sound is already playing. Don't overlap.
        return;
    }
    glm::vec3 cameraAt = ::g_pFlyCamera->getAt();
    FMOD_VECTOR soundPosition = { position.x, position.y, position.z };
    FMOD_VECTOR soundVelocity = { 0.f,0.f,0.f };
    FMOD_VECTOR listenerPosition = { ::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z };
    FMOD_VECTOR listenerForward = { cameraAt.x, cameraAt.y, cameraAt.z };
    FMOD_VECTOR listenerUp = { 0.f,1.f,0.f };

    _result = _system->set3DListenerAttributes(0, &listenerPosition, &soundVelocity, &listenerForward, &listenerUp);
    //Starts paused
    _result = _system->playSound(_sound, 0, true, &_channel);
    if (_result != FMOD_OK) {
        std::string output = "Unable to play sound 1";
        fprintf(stderr, output.c_str());
        return;
    }
    //Sets up 3D Attributes for Sound
    _result = _channel->set3DAttributes(&soundPosition, &soundVelocity);
    if (_result != FMOD_OK) {
        std::string output = "Unable to set 3d settings for channel 1";
        fprintf(stderr, output.c_str());
        return;
    }
    _result = _channel->setPaused(false);
    if (_result != FMOD_OK) {
        std::string output = "Unable to unpause channel 1";
        fprintf(stderr, output.c_str());
        return;
    }
}