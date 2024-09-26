/** WLVerse
 * \file AudioManager.h
 *
 * \brief
 *   Audio Manager extends a function to setup FMOD at the start and end of the game. This is meant
 *   to be used at the core level that manages its own construction and destruction.
 * 
 *   It doesn't handle any of the audio playing, and meant to be run in a system in the game layer.
 * 
 *   Now here's the sauce...
 *   For a quick start: https://www.fmod.com/docs/2.02/api/white-papers-getting-started.html#fmod-core-api-initialization-do-not-use-this-if-using-fmod-studio-api-initialization
 *   Core API reference: https://www.fmod.com/docs/2.02/api/core-api-system.html#system_playsound
 * 
 * \authors
 *   Yew Chong (yewchong.k\@digipen.edu)
 * 
 * \par All content (c) 2024 DigiPen Institute of Technology Singapore. All rights reserved.
 */

#pragma once

#include "FMOD/core/fmod.hpp"

namespace FMOD
{
  class AudioManager
  {
    FMOD::System* fmod_system = NULL;
    FMOD_RESULT result;

    FMOD::Sound* sound;
    FMOD::Channel* channel;

    AudioManager();
    ~AudioManager();

  public:
    void Update();
    void CreateSound();
    void PlaySound();
  };
} // namespace FMOD