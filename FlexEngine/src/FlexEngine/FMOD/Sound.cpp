#pragma once

#include "AudioManager.h"
#include "Sound.h"
#include <FMOD/core/fmod_errors.h>
namespace FlexEngine
{
  namespace Asset
  {
    Sound::Sound(std::string const& key) : sound(nullptr)
    {
      std::string path = "assets/" + key;
      FMOD_RESULT r = AudioManager::fmod_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound);
      if (r != FMOD_OK)
      {
        Log::Error("Failed to load sound: " + key);
        exit(0);
      }
    }
  }
}