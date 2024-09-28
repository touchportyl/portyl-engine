#pragma once

#include "AudioManager.h"
#include "Sound.h"

namespace FlexEngine
{
  namespace Asset
  {
    Sound::Sound(std::string const& key) : sound(nullptr)
    {
      std::string path = "assets" + key;
      FMOD_ASSERT(AudioManager::fmod_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound));
    }

    Sound::~Sound() noexcept
    {
      //Log::Fatal("Sound destructor called");
      //sound->release();
    }
  }
}