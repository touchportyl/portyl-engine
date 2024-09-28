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

#include "flx_api.h"
#include "pch.h"
#include "FMOD/core/fmod.hpp"
#include "FMOD/Sound.h"

namespace FlexEngine
{
class __FLX_API AudioManager
{
public:

  // This class extends usage for middle level to call from.
  class Core
  {
    static std::map<std::string, FMOD::Channel*> channels; // Stores all instances of channels in existence

  public:
    static void PlaySound(std::string const&, Asset::Sound const&);
    static void StopSound(std::string const&);
  };

  // Static fmod variables
  static FMOD::System* fmod_system; // NULL
  static FMOD_RESULT result;

  inline static bool is_init() { return fmod_system != NULL; }

  // Functions for setup of core FMOD systems
  static void Load();
  static void Unload();
  static void Update();
};
}// namespace FlexEngine