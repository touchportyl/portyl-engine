/** WLVerse
 * \file AudioManager.h
 *
 * \brief
 *   Audio wrapper to be used to play all audio in the game.
 *   This system uses FMOD studio as a basis for playing music
 *
 * \authors
 *   Yew Chong (yewchong.k\@digipen.edu)
 *
 * \link <https://portylstudios.notion.site/WLVerse-GAM150-Plan-d3460d6a24db417ca86f9ed54deaf365?pvs=4>
 *   WLVerse GAM150 Plan
 * \endlink
 *
 * \par All content (c) 2024 DigiPen Institute of Technology Singapore. All rights reserved.
 */

#include "AudioManager.h"

#include "fmod_errors.h"
#include <string>
#include <cassert>

using namespace FMOD; // To be in the scope of the .h file...

// Most of the FMOD functions return an FMOD_RESULT, we want to watch for this with FMOD assert macro to crash fast for debugging
#define FMOD_ASSERT(FUNCTION) \
          AudioManager::result = FUNCTION;  \
          assert(AudioManager::result == FMOD_OK && FMOD_ErrorString(AudioManager::result))

/*!
  \brief Constructor for the AudioManager. Handles the creation of the FMOD system.
*/
AudioManager::AudioManager()
{
  FMOD_ASSERT(FMOD::System_Create(&fmod_system));
  FMOD_ASSERT(fmod_system->init(32, FMOD_INIT_NORMAL, 0));
}

/*!
  \brief Destructor for the AudioManager. Handles close and release of the FMOD system.
*/
AudioManager::~AudioManager()
{
  fmod_system->close();
  fmod_system->release();
}

/*!
  \brief Updates the FMOD instance
*/
void AudioManager::Update()
{
  fmod_system->update();
}

/*!
  \brief Creates a sound from the audio file.
*/
void AudioManager::CreateSound()
{
  // FMOD returns a handle to the sound via the sound parameter
  fmod_system->createSound("audio.mp3", FMOD_DEFAULT, 0, &sound);
}

/*!
  \brief Plays the sound.
*/
void AudioManager::PlaySound()
{
  // Play the sound given a sound handle and a channel...
  // Channel group 0, channel to play from via the channel handle
  fmod_system->playSound(sound, 0, false, &channel);

  // Sample code to get if the channel is playing...
  bool b;
  channel->isPlaying(&b);
}